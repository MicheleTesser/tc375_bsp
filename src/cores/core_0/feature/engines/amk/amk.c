#include "./amk.h"
#include "serial.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include "../../../../../lib/board_dbc/dbc/out_lib/can1/can1.h"
#include "../../../../../lib/board_dbc/dbc/out_lib/can2/can2.h"
#pragma GCC diagnostic pop 
#include "../../../../../lib/raceup_board/raceup_board.h"
#include "../../../../core_utility/core_utility.h"
#include "../../math_saturated/saturated.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

//INFO: doc/amk_datasheet.pdf page 61

//private

float MAX_MOTOR_TORQUE = 21.0f; //INFO: default 21.0f;
uint16_t SPEED_LIMIT = 20000;  //INFO: default 20000;

FlashMemoryBank_h M_RPM_FLASH_DATA;
FlashMemoryBank_h M_MAX_POS_TRQ_FLASH_DATA;

enum AMK_EMERGENCY {
  FAILED_RTD_AMK =0,

  __NUM_OF_AMK_EMERGENCY__
};

struct AMK_Actual_Values_1{
  struct{
    uint8_t b_reserve; //INFO: Reserved
    uint8_t AMK_bSystemReady: 1; //INFO: System ready (SBM)
    uint8_t AMK_bError :1; //INFO: Error
    uint8_t AMK_bWarn :1; //INFO: Warning
    uint8_t AMK_bQuitDcOn :1; //INFO: HV activation acknowledgment
    uint8_t AMK_bDcOn :1; //INFO: HV activation level
    uint8_t AMK_bQuitInverterOn:1; //INFO: Controller enable acknowledgment
    uint8_t AMK_bInverterOn:1; //INFO: Controller enable level
    uint8_t AMK_bDerating :1; //INFO: Derating (torque limitation active)
  }AMK_status;
  //INFO:  AMK_ActualVelocity : Unit = rpm; Actual speed value
  int16_t AMK_ActualVelocity; 
  //INFO: AMK_TorqueCurrent: Raw data for calculating 'actual torque current' Iq See 'Units' on page 65.               
  int16_t AMK_TorqueCurrent; 
  //INFO: Raw data for calculating 'actual magnetizing current' Id See 'Units' on page 1.
  int16_t AMK_MagnetizingCurrent;
};

struct AMK_Actual_Values_2{
  float AMK_TempMotor; //INFO: Unit= 0.1 °C. Motor temperature
  float AMK_TempInverter; //INFO: Unit= 0.1 °C. Cold plate temperature
  float AMK_ErrorInfo; //INFO: Diagnostic number
  float AMK_TempIGBT; //INFO: 0.1 °C. IGBT temperature
};

struct AMK_Setpoints{
  //INFO: AMK_Control: 
  //Control word See the table below: Content of the 'AMK_Control' control word
  struct {
    uint8_t AMK_bReserve_start;
    uint8_t AMK_bInverterOn:1;
    uint8_t AMK_bDcOn:1;
    uint8_t AMK_bEnable:1;
    uint8_t AMK_bErrorReset:1;
    uint8_t AMK_bReserve_end :4;
  }AMK_Control_fields;
  int16_t AMK_TargetVelocity; //INFO: unit: rpm Speed setpoint
  int16_t AMK_TorqueLimitPositive; //INFO: unit: 0.1% MN Positive torque limit (subject to nominal torque)
  int16_t AMK_TorqueLimitNegative; //INFO: unit: 0.1% MN Negative torque limit (subject to nominal torque)
};

typedef struct Inverter{
  CarSpeedMut_h m_car_speed;
  uint8_t rf_status:1;
  uint8_t started_precharge:1;
  uint8_t hvCounter[__NUM_OF_ENGINES__];
  struct EngineInfo{
    struct AMK_Actual_Values_1 amk_values_1;
    struct AMK_Actual_Values_2 amk_values_2;
    int16_t AMK_TargetVelocity; // INFO: unit: rpm Speed setpoint
    int16_t AMK_TorqueLimitPositive; //INFO: unit: 0.1% MN Positive torque limit (subject to nominal torque)
    int16_t AMK_TorqueLimitNegative; //INFO: unit: 0.1% MN Negative torque limit (subject to nominal torque)
  }engines[__NUM_OF_ENGINES__];
  time_var_microseconds u_last_send_info; 
  time_var_microseconds m_change_stage_req; 
  GpioRead_h gpio_precharge_init; 
  GpioRead_h gpio_precharge_done; 
  GpioRead_h gpio_rtd_button; 
  enum RUNNING_STATUS engine_status; 
  const DriverInput_h* driver_input; 
  struct CanNode* can_inverter; 
  struct{
    struct CanMailbox* p_recv_mailbox_inverter_1; 
    struct CanMailbox* p_recv_mailbox_inverter_2; 
  }m_engines_mailbox[__NUM_OF_ENGINES__];
  struct CanMailbox* mailbox_pcu_rf_signal_send;
  struct CanMailbox* mailbox_pcu_rf_signal_read;

  EmergencyNode_h __attribute__((aligned(4))) amk_emergency; 
}AMKInverter_t;

union AMKConv{
  AmkInverter_h* hidden;
  AMKInverter_t* clear;
};

#define AMK_H_T_CONV(h_ptr, t_ptr_name)\
  union AMKConv __a_conv##t_ptr_name__ = {h_ptr};\
  AMKInverter_t* t_ptr_name = __a_conv##t_ptr_name__.clear;

#ifdef DEBUG
// char (*aa)[sizeof(AMKInverter_t)]=1;
const uint8_t __debug_amk_size__[(sizeof(AmkInverter_h) == sizeof(AMKInverter_t))? 1 : -1];
const uint8_t __debug_amk_align__[(_Alignof(AmkInverter_h) == _Alignof(AMKInverter_t))? 1 : -1];
#endif /* ifdef DEBUG */

#define POPULATE_MEX_ENGINE(amk_stop,engine)\
  engine.AMK_bReserved = 0;\
engine.AMK_bDcOn = amk_stop->AMK_Control_fields.AMK_bDcOn;\
engine.AMK_bInverterOn = amk_stop->AMK_Control_fields.AMK_bInverterOn;\
engine.AMK_bEnable = amk_stop->AMK_Control_fields.AMK_bEnable;\
engine.AMK_bErrorReset = amk_stop->AMK_Control_fields.AMK_bErrorReset;\
engine.AMK_bReservedEnd = 0;\
engine.NegTorq = amk_stop->AMK_TorqueLimitNegative;\
engine.PosTorq = amk_stop->AMK_TorqueLimitPositive;\
engine.TargetVel = amk_stop->AMK_TargetVelocity;\

static int8_t _dps_post_update_speed_limit(const char* const var_name, const void* const var)
{
  uint64_t data_flash;
  (void) var_name;

  memcpy(&data_flash,var,sizeof(SPEED_LIMIT));

  return flash_memory_write_bank(&M_RPM_FLASH_DATA, (uint32_t*)&data_flash);
}

static int8_t _dps_post_update_max_pos_trq(const char* const var_name, const void* const var)
{
  uint64_t data_flash;
  (void) var_name;

  memcpy(&data_flash,var,sizeof(MAX_MOTOR_TORQUE));

  return flash_memory_write_bank(&M_MAX_POS_TRQ_FLASH_DATA, (uint32_t*)&data_flash);
}

static int8_t _send_message_amk(const AMKInverter_t* const restrict self,
    const enum ENGINES engine, 
    const struct AMK_Setpoints* const restrict setpoint)
{
  can_obj_can1_h_t om={0};
  CanMessage mex={0};

  switch (engine)
  {
    case FRONT_LEFT:
      POPULATE_MEX_ENGINE(setpoint, om.can_0x189_VCUInvFL);
      mex.id = CAN_ID_VCUINVFL;
      break;
    case FRONT_RIGHT:
      POPULATE_MEX_ENGINE(setpoint, om.can_0x188_VCUInvFR);
      mex.id = CAN_ID_VCUINVFR;
      break;
    case REAR_LEFT:
      POPULATE_MEX_ENGINE(setpoint, om.can_0x185_VCUInvRL);
      mex.id = CAN_ID_VCUINVRL;
      break;
    case REAR_RIGHT:
      POPULATE_MEX_ENGINE(setpoint, om.can_0x184_VCUInvRR);
      mex.id = CAN_ID_VCUINVRR;
      break;
    default:
      SET_TRACE(CORE_0, UNREACHABLE_CODE);
      return -1;
  }

  mex.message_size = (uint8_t) pack_message_can1(&om, mex.id, &mex.full_word);
  return hardware_write_can(self->can_inverter, &mex);
}

static uint8_t _amk_inverter_on(const AMKInverter_t* const restrict self)
{
  uint8_t res = 0;
  FOR_EACH_ENGINE(engine){
    res |= self->engines[engine].amk_values_1.AMK_status.AMK_bSystemReady;
  }
  return res;
}

static uint8_t _precharge_ended(const AMKInverter_t* const restrict self)
{
  uint8_t res =0;
  FOR_EACH_ENGINE(engine)
  {
    res |= self->engines[engine].amk_values_1.AMK_status.AMK_bQuitDcOn;
  }
  return 
    res &&
    self->started_precharge &&
    !gpio_read_state(&self->gpio_precharge_init) &&
    gpio_read_state(&self->gpio_precharge_done);
}

static uint8_t _precharge_started(const AMKInverter_t* const restrict self)__attribute__((__unused__));
static uint8_t _precharge_started(const AMKInverter_t* const restrict self)
{
  uint8_t res =0;
  FOR_EACH_ENGINE(engine)
  {
    res |= self->engines[engine].amk_values_1.AMK_status.AMK_bDcOn;
  }
  return res &&
    !gpio_read_state(&self->gpio_precharge_init) && !gpio_read_state(&self->gpio_precharge_done);
}

static uint8_t _amk_inverter_hv_status(AMKInverter_t* const restrict self)
{
  const uint8_t HV_TRAP = 50;
  uint8_t res = 0;

  FOR_EACH_ENGINE(engine)
  {
    const uint8_t AMK_bQuitDcOn = 
      self->engines[engine].amk_values_1.AMK_status.AMK_bQuitDcOn;
    if (!(AMK_bQuitDcOn) && (self->hvCounter[engine] < HV_TRAP))
    {
      self->hvCounter[engine]++;
    }
    else if (AMK_bQuitDcOn || (self->hvCounter[engine] >= HV_TRAP))
    {
      res |= AMK_bQuitDcOn;
      self->hvCounter[engine] = 0;
    }
  }


  return res;
}

static void _amk_rtd_next_step(AMKInverter_t* const restrict self)
{
  if (!self->m_change_stage_req)
  {
    self->m_change_stage_req = timer_time_now();
  }
  else if((timer_time_now() - self->m_change_stage_req)> get_tick_from_millis(10))
  {
    self->engine_status = (self->engine_status+1)%(RUNNING+1);
    self->m_change_stage_req=0;
  }
}

static void _amk_update_rtd_procedure(AMKInverter_t* const restrict self)
{
  struct AMK_Setpoints setpoint = {0};
  can_obj_can2_h_t o2={0};
  uint64_t data=0;
  uint8_t send_pcu=0;

  if(self->engine_status == SYSTEM_OFF)
  {
    EmergencyNode_solve(&self->amk_emergency, FAILED_RTD_AMK);
  }

  switch (self->engine_status)
  {
    case SYSTEM_OFF:
      self->started_precharge = 0;
      self->rf_status= 0;
      if (_amk_inverter_on(self))
      {
        setpoint.AMK_Control_fields.AMK_bDcOn = 1;
      }
      else
      {
        setpoint.AMK_Control_fields.AMK_bErrorReset = 1;
      }

      if (!EmergencyNode_is_emergency_state(&self->amk_emergency) &&
          _amk_inverter_on(self) &&
          _precharge_started(self) &&
          !giei_driver_input_rtd_request(self->driver_input))
      {
        _amk_rtd_next_step(self);
      }
      else
      {
#ifdef BOARD_ISOLATED
#pragma message "board is isoltaed ignoring rtd input req in amk for faults"
        if(EmergencyNode_is_emergency_state(&self->amk_emergency))
#else
        if(
            giei_driver_input_rtd_request(self->driver_input) ||
            EmergencyNode_is_emergency_state(&self->amk_emergency))
#endif
        {
          EmergencyNode_raise(&self->amk_emergency, FAILED_RTD_AMK);
          self->engine_status = SYSTEM_OFF;
          return;
        }
      }
      break;
    case SYSTEM_PRECAHRGE:
      self->started_precharge = 1;
      self->rf_status= 0;
      setpoint.AMK_Control_fields.AMK_bDcOn = 1;
      if (!_amk_inverter_on(self) ||
          giei_driver_input_rtd_request(self->driver_input))
      {
        SET_TRACE(CORE_0, RTD_PROCEDURE_FAILED);
        EmergencyNode_raise(&self->amk_emergency, FAILED_RTD_AMK);
        memset(&setpoint, 0, sizeof(setpoint));
        self->engine_status = SYSTEM_OFF;
      }
      else if(giei_driver_input_rtd_request(self->driver_input) ||
            EmergencyNode_is_emergency_state(&self->amk_emergency))
      {
        SET_TRACE(CORE_0, RTD_PROCEDURE_FAILED);
        EmergencyNode_raise(&self->amk_emergency, FAILED_RTD_AMK);
        memset(&setpoint, 0, sizeof(setpoint));
        self->engine_status = SYSTEM_OFF;
      }

      else if (_precharge_ended(self))
      {
        _amk_rtd_next_step(self);
      }
      break;
    case TS_READY:

      setpoint.AMK_Control_fields.AMK_bDcOn = 1;
      setpoint.AMK_Control_fields.AMK_bInverterOn = 1;

      if (!_amk_inverter_hv_status(self) || !_precharge_ended(self) || !_amk_inverter_on(self))
      {
        SET_TRACE(CORE_0, RTD_PROCEDURE_FAILED);
        EmergencyNode_raise(&self->amk_emergency, FAILED_RTD_AMK);
        memset(&setpoint, 0, sizeof(setpoint));
        self->engine_status = SYSTEM_OFF;
      }
      else if (giei_driver_input_rtd_request(self->driver_input) &&
          giei_driver_input_get(self->driver_input, BRAKE) >= 95)
      {
        send_pcu=1;
        o2.can_0x130_Pcu.mode = 1;
        o2.can_0x130_Pcu.rf = giei_driver_input_rtd_request(self->driver_input);

        //INFO: RF message to PCU and wait until rf is active before going
        //into RUNNING status
        if (self->rf_status)
        {
          setpoint.AMK_Control_fields.AMK_bDcOn = 1;
          setpoint.AMK_Control_fields.AMK_bInverterOn = 1;
          setpoint.AMK_Control_fields.AMK_bEnable = 1;

          _amk_rtd_next_step(self);
        }
      }
      break;
    case RUNNING:
      o2.can_0x130_Pcu.rf = giei_driver_input_rtd_request(self->driver_input);
      if (!_amk_inverter_hv_status(self) ||
          //!_precharge_ended(self)||
          !_amk_inverter_on(self)||
          EmergencyNode_is_emergency_state(&self->amk_emergency))
      {
        SET_TRACE(CORE_0, ENGINE_ERROR);
        EmergencyNode_raise(&self->amk_emergency, FAILED_RTD_AMK);
        memset(&setpoint, 0, sizeof(setpoint));
        self->engine_status = SYSTEM_OFF;
      }
      else if (!giei_driver_input_rtd_request(self->driver_input))
      {
        send_pcu=1;
        o2.can_0x130_Pcu.mode = 1;
        o2.can_0x130_Pcu.rf = 0;
        setpoint.AMK_Control_fields.AMK_bDcOn = 1;
        setpoint.AMK_Control_fields.AMK_bInverterOn = 1;
        setpoint.AMK_Control_fields.AMK_bEnable = 0;
        self->engine_status= TS_READY;
      }
      else
      {
        return;
      }

      break;
  }

  if (send_pcu)
  {
    pack_message_can2(&o2, CAN_ID_PCU, &data);
    hardware_mailbox_send(self->mailbox_pcu_rf_signal_send, data);
  }

  ACTION_ON_FREQUENCY(self->u_last_send_info, get_tick_from_millis(10))
  {
    FOR_EACH_ENGINE(engine)
    {
      _send_message_amk(self, engine, &setpoint);
    }
  }

}

static int8_t _share_var_engine(const AMKInverter_t* const restrict self, const enum ENGINES engine)
{
  uint8_t basic_id_start = NS_SD(AMK_STATUS_FL);
  uint8_t cursor =0;

  switch (engine)
  {
    case FRONT_LEFT:
      basic_id_start = NS_SD(AMK_STATUS_FL);
      break;
    case FRONT_RIGHT:
      basic_id_start = NS_SD(AMK_STATUS_FR);
      break;
    case REAR_LEFT:
      basic_id_start = NS_SD(AMK_STATUS_RL);
      break;
    case REAR_RIGHT:
      basic_id_start = NS_SD(AMK_STATUS_RR);
      break;
    default:
      SET_TRACE(CORE_0, UNREACHABLE_CODE);
      return -1;
  }

#define SHARE_LOG_VAR(engine, var)\
  NS_SH(update)(basic_id_start + cursor, (SharedDataType)&self->engines[engine].var);++cursor;

  //INFO: ORDER MATTER. For order look at shared_memory.h in AMK_SHARED_VARS
  SHARE_LOG_VAR(engine, amk_values_1.AMK_status);
  SHARE_LOG_VAR(engine, amk_values_1.AMK_ActualVelocity);
  SHARE_LOG_VAR(engine, amk_values_1.AMK_TorqueCurrent);
  SHARE_LOG_VAR(engine, amk_values_1.AMK_MagnetizingCurrent);
  SHARE_LOG_VAR(engine, amk_values_1.AMK_TorqueCurrent);
  SHARE_LOG_VAR(engine, amk_values_2.AMK_TempMotor);
  SHARE_LOG_VAR(engine, amk_values_2.AMK_TempInverter);
  SHARE_LOG_VAR(engine, amk_values_2.AMK_TempIGBT);
  SHARE_LOG_VAR(engine, amk_values_2.AMK_ErrorInfo);
  SHARE_LOG_VAR(engine, AMK_TorqueLimitPositive);
  SHARE_LOG_VAR(engine, AMK_TorqueLimitNegative);

  return 0;
}

static void _amk_log_var(const AMKInverter_t* const restrict self, const enum ENGINES engine)__attribute__((__unused__));
static void _amk_log_var(const AMKInverter_t* const restrict self, const enum ENGINES engine)
{
  const struct EngineInfo* p_engine = &self->engines[engine];
  char engine_str[] = "fl";

  switch (engine)
  {
    case FRONT_LEFT:
      engine_str[0] = 'f';
      engine_str[1] = 'l';
      break;
    case FRONT_RIGHT:
      engine_str[0] = 'f';
      engine_str[1] = 'r';
      break;
    case REAR_LEFT:
      engine_str[0] = 'r';
      engine_str[1] = 'l';
      break;
    case REAR_RIGHT:
      engine_str[0] = 'r';
      engine_str[1] = 'r';
      break;
    default:
      return;
  }

#define PUSH_VAR(PTR,FORMAT, MODE, NAME)\
  {\
    LogEntry_h entry = {0};\
    char* cursor = NULL;\
    cursor=NULL;\
    entry.log_mode = LOG_TELEMETRY | LOG_SD;\
    entry.data_ptr = PTR;\
    entry.data_format= FORMAT;\
    entry.data_mode = MODE;\
    cursor = entry.name;\
    cursor = strcat(cursor, NAME);\
    strcat(cursor, engine_str);\
    while (log_add_entry(&entry)<0);\
  }


  PUSH_VAR(&p_engine->amk_values_1.AMK_ActualVelocity, SIGNED,__i16__,"amk_actual_velocity_");
  PUSH_VAR(&p_engine->amk_values_1.AMK_TorqueCurrent, UNSIGNED,__u16__,"amk_torque_current_");
  PUSH_VAR(&p_engine->amk_values_1.AMK_MagnetizingCurrent, UNSIGNED,__u16__,"amk_magnetizing_current_");
  PUSH_VAR(&p_engine->amk_values_1.AMK_status, UNSIGNED,__u16__,"amk_status_");

  PUSH_VAR(&p_engine->amk_values_2.AMK_ErrorInfo, FLOAT,__float__,"amk_error_info_");
  PUSH_VAR(&p_engine->amk_values_2.AMK_TempInverter, FLOAT,__float__,"amk_temp_inverter_");
  PUSH_VAR(&p_engine->amk_values_2.AMK_TempIGBT, FLOAT,__float__,"amk_temp_igbt_");
  PUSH_VAR(&p_engine->amk_values_2.AMK_TempMotor, FLOAT,__float__,"amk_temp_motor_");

  PUSH_VAR(&p_engine->AMK_TargetVelocity, SIGNED,__i16__,"amk_target_velocity_");
  PUSH_VAR(&p_engine->AMK_TorqueLimitPositive, SIGNED,__i16__,"amk_torque_limit_positive_");
  PUSH_VAR(&p_engine->AMK_TorqueLimitNegative, SIGNED,__i16__,"amk_torque_limit_negative_");
}

//public

enum RUNNING_STATUS amk_rtd_procedure(const AMKInverter_t* const restrict self)
{
  return self->engine_status;
}

#define UPDATE_INFO_1(engine, mex)\
{\
  struct AMK_Actual_Values_1* val = &self->engines[engine].amk_values_1;\
  val->AMK_ActualVelocity = mex.ActualVelocity;\
  val->AMK_MagnetizingCurrent = mex.MagCurr;\
  val->AMK_TorqueCurrent = mex.Voltage;\
  val->AMK_status.AMK_bSystemReady= mex.SystemReady;\
  val->AMK_status.AMK_bDcOn = mex.HVOn;\
  val->AMK_status.AMK_bQuitDcOn = mex.HVOnAck;\
  val->AMK_status.AMK_bInverterOn = mex.InverterOn;\
  val->AMK_status.AMK_bQuitInverterOn = mex.InverterOnAck;\
  val->AMK_status.AMK_bDerating= mex.Derating;\
  val->AMK_status.AMK_bWarn = mex.Warning;\
  val->AMK_status.AMK_bError = mex.Error;\
}

#define UPDATE_INFO_2(engine, mex)\
{\
  struct AMK_Actual_Values_2* val = &self->engines[engine].amk_values_2;\
  val->AMK_ErrorInfo = mex.ErrorInfo;\
  val->AMK_TempIGBT = mex.TempIGBT/10;\
  val->AMK_TempInverter = mex.TempInv/10;\
  val->AMK_TempMotor = mex.TempMotor/10;\
}

int8_t amk_update(AMKInverter_t* const restrict self)
{
  int8_t err=0;
  CanMessage mex = {0};
  can_obj_can1_h_t o1 ={0};
  can_obj_can2_h_t o2 ={0};

  _amk_update_rtd_procedure(self);


#define READ_MAILBOX_INV(ENGINE, ID_1, ID_2,  MEX_1, MEX_2)\
  if (hardware_mailbox_read(self->m_engines_mailbox[ENGINE].p_recv_mailbox_inverter_1, &mex)>0)\
  {\
    if(unpack_message_can1(&o1, ID_1, mex.full_word, 8, timer_time_now())<0)\
    {\
      SET_TRACE(CORE_0, UNPACK_ERROR);\
    }\
    else\
    {\
      UPDATE_INFO_1(ENGINE, o1.MEX_1);\
    }\
  }\
  if (hardware_mailbox_read(self->m_engines_mailbox[ENGINE].p_recv_mailbox_inverter_2, &mex)>0)\
  {\
    if(unpack_message_can1(&o1, ID_2, mex.full_word, 8, timer_time_now())<0)\
    {\
      SET_TRACE(CORE_0, UNPACK_ERROR);\
    }\
    else\
    {\
      UPDATE_INFO_2(ENGINE, o1.MEX_2);\
    }\
  }

  READ_MAILBOX_INV(
      FRONT_LEFT,
      CAN_ID_INVERTERFL1,
      CAN_ID_INVERTERFL2,
      can_0x288_InverterFL1,
      can_0x28a_InverterFL2);

  READ_MAILBOX_INV(
      FRONT_RIGHT,
      CAN_ID_INVERTERFR1,
      CAN_ID_INVERTERFR2,
      can_0x287_InverterFR1,
      can_0x289_InverterFR2);

  READ_MAILBOX_INV(
      REAR_LEFT,
      CAN_ID_INVERTERRL1,
      CAN_ID_INVERTERRL2,
      can_0x284_InverterRL1,
      can_0x286_InverterRL2);

  READ_MAILBOX_INV(
      REAR_RIGHT,
      CAN_ID_INVERTERRR1,
      CAN_ID_INVERTERRR2,
      can_0x283_InverterRR1,
      can_0x285_InverterRR2);

  const float engines_rpm[] = 
  {
    self->engines[FRONT_LEFT].amk_values_1.AMK_ActualVelocity,
    self->engines[FRONT_RIGHT].amk_values_1.AMK_ActualVelocity,
    self->engines[REAR_LEFT].amk_values_1.AMK_ActualVelocity,
    self->engines[REAR_RIGHT].amk_values_1.AMK_ActualVelocity,
  };
  if(car_speed_mut_update(&self->m_car_speed, engines_rpm)<0)
  {
    err--;
  }

  memset(&mex, 0, sizeof(mex));
  if (hardware_mailbox_read(self->mailbox_pcu_rf_signal_read, &mex)>0)
  {
    unpack_message_can2(
        &o2,
        CAN_ID_PCURFACK,
        mex.full_word,
        (uint8_t)message_dlc_can2(CAN_ID_PCURFACK),
        0);
    self->rf_status = o2.can_0x134_PcuRfAck.rf_signalAck;
  }

  return err;
}

float amk_get_info(const AMKInverter_t* const restrict self,
    const enum ENGINES engine, const enum ENGINE_INFO info)
{
  if (engine == __NUM_OF_ENGINES__) {
    return -1;
  }
  switch (info)
  {
    case ENGINE_VOLTAGE:
      return self->engines[engine].amk_values_1.AMK_TorqueCurrent;
    case ENGINE_RPM:
      return self->engines[engine].amk_values_1.AMK_ActualVelocity;
    case TARGET_VELOCITY:
      return self->engines[engine].AMK_TargetVelocity;
    default:
      SET_TRACE(CORE_0, UNREACHABLE_CODE);
      return -1;
  }
}

int8_t amk_send_torque(AMKInverter_t* const restrict self,
    const enum ENGINES engine, const float pos_torque, const float neg_torque)
{
  struct AMK_Setpoints setpoint  ={
    .AMK_Control_fields ={0},
    .AMK_TargetVelocity = 0,
    .AMK_TorqueLimitPositive = 0,
    .AMK_TorqueLimitNegative = 0,
  };

  if (amk_rtd_procedure(self) == RUNNING)
  {
    setpoint.AMK_TargetVelocity = 0;
    if (pos_torque > 0)
    {
      setpoint.AMK_TargetVelocity = SPEED_LIMIT;
    }
    setpoint.AMK_Control_fields.AMK_bDcOn=1;
    setpoint.AMK_Control_fields.AMK_bEnable=1;
    setpoint.AMK_Control_fields.AMK_bInverterOn =1;
    setpoint.AMK_TorqueLimitPositive = (int16_t)(pos_torque / 0.0098f); //INFO: magic number from dbc
    setpoint.AMK_TorqueLimitNegative = (int16_t)(neg_torque / 0.0098f); //INFO: magic number from dbc
  }

  self->engines[engine].AMK_TorqueLimitNegative = setpoint.AMK_TorqueLimitNegative;
  self->engines[engine].AMK_TorqueLimitPositive = setpoint.AMK_TorqueLimitPositive;
  

  return _send_message_amk(self, engine, &setpoint);
}

void amk_destroy(AMKInverter_t* const restrict p_self)
{
  hardware_free_mailbox_can(&p_self->m_engines_mailbox[REAR_RIGHT].p_recv_mailbox_inverter_2);
  hardware_free_mailbox_can(&p_self->m_engines_mailbox[REAR_RIGHT].p_recv_mailbox_inverter_1);
  hardware_free_mailbox_can(&p_self->m_engines_mailbox[REAR_LEFT].p_recv_mailbox_inverter_2);
  hardware_free_mailbox_can(&p_self->m_engines_mailbox[REAR_LEFT].p_recv_mailbox_inverter_1);
  hardware_free_mailbox_can(&p_self->m_engines_mailbox[FRONT_RIGHT].p_recv_mailbox_inverter_2);
  hardware_free_mailbox_can(&p_self->m_engines_mailbox[FRONT_RIGHT].p_recv_mailbox_inverter_1);
  hardware_free_mailbox_can(&p_self->m_engines_mailbox[FRONT_LEFT].p_recv_mailbox_inverter_2);
  hardware_free_mailbox_can(&p_self->m_engines_mailbox[FRONT_LEFT].p_recv_mailbox_inverter_1);
  hardware_free_mailbox_can(&p_self->mailbox_pcu_rf_signal_read);
  hardware_free_mailbox_can(&p_self->mailbox_pcu_rf_signal_send);
  gpio_read_destroy(&p_self->gpio_rtd_button);
  gpio_read_destroy(&p_self->gpio_precharge_done);
  gpio_read_destroy(&p_self->gpio_precharge_init);
  car_speed_mut_destroy(&p_self->m_car_speed);
  hardware_init_can_destroy_ref_node(&p_self->can_inverter);
  EmergencyNode_destroy(&p_self->amk_emergency);

  return;
}

void amk_reset(AMKInverter_t* const restrict p_self)
{
  p_self->engine_status = SYSTEM_OFF;
}

int8_t amk_module_init(AmkInverter_h* const restrict self,
    const DriverInput_h* const p_driver_input,
    struct EngineType* const restrict general_inverter)
{
  AMK_H_T_CONV(self, p_self);
  struct CanNode* can_node = NULL;
  int8_t err=0;
  uint64_t flash_data __attribute__((__unused__)) =0;

  memset(p_self, 0, sizeof(*p_self));
  p_self->engine_status =SYSTEM_OFF;
  p_self->driver_input = p_driver_input;

  if (EmergencyNode_init(&p_self->amk_emergency))
  {
    goto EmergencyNode_failed;
  }

  p_self->can_inverter = hardware_init_can_get_ref_node(CAN_INVERTER);
  if (!p_self->can_inverter)
  {
    goto can_inverter_failed;
  }

  if (car_speed_mut_init(&p_self->m_car_speed)<0)
  {
    goto car_speed_init_failed;
  }

  if (hardware_init_read_permission_gpio(&p_self->gpio_precharge_init, GPIO_AIR_PRECHARGE_INIT)<0)
 
  {
    goto read_gpio_precharge_init_failed;
  }

  if (hardware_init_read_permission_gpio(&p_self->gpio_precharge_done, GPIO_AIR_PRECHARGE_DONE)<0)
  {
    goto read_gpio_precharge_done_failed;
  }

  if (hardware_init_read_permission_gpio(&p_self->gpio_rtd_button, GPIO_RTD_BUTTON)<0)
  {
    goto read_gpio_rtd_button_failed;
  }

  ACTION_ON_CAN_NODE(CAN_GENERAL,can_node)
  {
    p_self->mailbox_pcu_rf_signal_send =
      hardware_get_mailbox_single_mex(can_node,
          SEND_MAILBOX,
          CAN_ID_PCU,
          (uint16_t)message_dlc_can2(CAN_ID_PCU));
  }

  if (!p_self->mailbox_pcu_rf_signal_send)
  {
    goto mailbox_pcu_send_failed;
  }

  ACTION_ON_CAN_NODE(CAN_GENERAL,can_node)
  {
    p_self->mailbox_pcu_rf_signal_read =
      hardware_get_mailbox_single_mex(
          can_node,
          RECV_MAILBOX,
          CAN_ID_PCURFACK,
          (uint16_t)message_dlc_can2(CAN_ID_PCURFACK));
  }

  if (!p_self->mailbox_pcu_rf_signal_read)
  {
    goto mailbox_pcu_read_failed;
  }

#define INVERTER_MAILBOX(engine, MAILBOX, CAN_ID, ERROR)\
  p_self->m_engines_mailbox[engine].MAILBOX =\
    hardware_get_mailbox_single_mex(\
        p_self->can_inverter,\
        RECV_MAILBOX,\
        CAN_ID,\
        (uint16_t)message_dlc_can1(CAN_ID));\
  if(!p_self->m_engines_mailbox[engine].MAILBOX) goto ERROR;


  INVERTER_MAILBOX(FRONT_LEFT, p_recv_mailbox_inverter_1, CAN_ID_INVERTERFL1,mailbox_inv_fl_1_failed)
  INVERTER_MAILBOX(FRONT_LEFT, p_recv_mailbox_inverter_2, CAN_ID_INVERTERFL2,mailbox_inv_fl_2_failed)

  INVERTER_MAILBOX(FRONT_RIGHT, p_recv_mailbox_inverter_1, CAN_ID_INVERTERFR1,mailbox_inv_fr_1_failed)
  INVERTER_MAILBOX(FRONT_RIGHT, p_recv_mailbox_inverter_2, CAN_ID_INVERTERFR2,mailbox_inv_fr_2_failed)

  INVERTER_MAILBOX(REAR_LEFT, p_recv_mailbox_inverter_1, CAN_ID_INVERTERRL1,mailbox_inv_rl_1_failed)
  INVERTER_MAILBOX(REAR_LEFT, p_recv_mailbox_inverter_2, CAN_ID_INVERTERRL2,mailbox_inv_rl_2_failed)

  INVERTER_MAILBOX(REAR_RIGHT, p_recv_mailbox_inverter_1, CAN_ID_INVERTERRR1,mailbox_inv_rr_1_failed)
  INVERTER_MAILBOX(REAR_RIGHT, p_recv_mailbox_inverter_2, CAN_ID_INVERTERRR2,mailbox_inv_rr_2_failed)

  while (flash_memory_req_bank(&M_RPM_FLASH_DATA, 1)<0)
  {
    serial_write_str("req flash bank rpm failed");
    timer_wait(get_tick_from_millis(100));
  }

  flash_memory_read_bank(&M_RPM_FLASH_DATA, (uint32_t*)&flash_data);
  SPEED_LIMIT = (uint16_t) flash_data;

  if (!SPEED_LIMIT)
  {
    SPEED_LIMIT = 20.000;
    memcpy(&flash_data, &SPEED_LIMIT, sizeof(SPEED_LIMIT));
    flash_memory_write_bank(&M_RPM_FLASH_DATA, (uint32_t*)&flash_data);
  }


  while (flash_memory_req_bank(&M_MAX_POS_TRQ_FLASH_DATA, 2)<0)
  {
    serial_write_str("req flash bank max pos trq failed");
    timer_wait(get_tick_from_millis(100));
  }

  flash_memory_read_bank(&M_MAX_POS_TRQ_FLASH_DATA, (uint32_t*)&flash_data);
  memcpy(&MAX_MOTOR_TORQUE, &flash_data, sizeof(MAX_MOTOR_TORQUE));

  if (MAX_MOTOR_TORQUE == 0.0f)
  {
    MAX_MOTOR_TORQUE = 21.0f;
    memcpy(&flash_data, &MAX_MOTOR_TORQUE, sizeof(MAX_MOTOR_TORQUE));
    flash_memory_write_bank(&M_MAX_POS_TRQ_FLASH_DATA, (uint32_t*)&flash_data);
  }


  serial_write_uint16_t(SPEED_LIMIT);
  serial_write_str("");
  serial_write_float(MAX_MOTOR_TORQUE);
  serial_write_str("");

  FOR_EACH_ENGINE(engine)
  {
    if((err =_share_var_engine(p_self, engine))<0)
    {
      err += -9;
      goto shared_var_engine_failed;
    } else {
    }
  }


  char Mrpm[VAR_NAME_LENGTH] = "Mrpm";
  char Mtorque[VAR_NAME_LENGTH] = "Mtrq";

  while(system_settings_push_var(
        DPS_TYPES_UINT16_T,
        &SPEED_LIMIT,
        _dps_post_update_speed_limit,
        Mrpm)<0);

  while(system_settings_push_var(
        DPS_TYPES_FLOAT_T,
        &MAX_MOTOR_TORQUE,
        _dps_post_update_max_pos_trq,
        Mtorque)<0);

  FOR_EACH_ENGINE(engine)
  {
    _amk_log_var(p_self,engine );
  }

  general_inverter->update_f=amk_update;
  general_inverter->rtd_f=amk_rtd_procedure;
  general_inverter->get_info_f=amk_get_info;
  general_inverter->send_torque_f=amk_send_torque;
  general_inverter->destroy_f=amk_destroy;
  general_inverter->reset_f=amk_reset;
  general_inverter->data = p_self;



  return 0;


shared_var_engine_failed:
  err--;
  hardware_free_mailbox_can(&p_self->m_engines_mailbox[REAR_RIGHT].p_recv_mailbox_inverter_2);
mailbox_inv_rr_2_failed:
  err--;
  hardware_free_mailbox_can(&p_self->m_engines_mailbox[REAR_RIGHT].p_recv_mailbox_inverter_1);
mailbox_inv_rr_1_failed:
  err--;
  hardware_free_mailbox_can(&p_self->m_engines_mailbox[REAR_LEFT].p_recv_mailbox_inverter_2);
mailbox_inv_rl_2_failed:
  err--;
  hardware_free_mailbox_can(&p_self->m_engines_mailbox[REAR_LEFT].p_recv_mailbox_inverter_1);
mailbox_inv_rl_1_failed:
  err--;
  hardware_free_mailbox_can(&p_self->m_engines_mailbox[FRONT_RIGHT].p_recv_mailbox_inverter_2);
mailbox_inv_fr_2_failed:
  err--;
  hardware_free_mailbox_can(&p_self->m_engines_mailbox[FRONT_RIGHT].p_recv_mailbox_inverter_1);
mailbox_inv_fr_1_failed:
  err--;
  hardware_free_mailbox_can(&p_self->m_engines_mailbox[FRONT_LEFT].p_recv_mailbox_inverter_2);
mailbox_inv_fl_2_failed:
  err--;
  hardware_free_mailbox_can(&p_self->m_engines_mailbox[FRONT_LEFT].p_recv_mailbox_inverter_1);
mailbox_inv_fl_1_failed:
  err--;
  hardware_free_mailbox_can(&p_self->mailbox_pcu_rf_signal_read);
mailbox_pcu_read_failed:
  err--;
  hardware_free_mailbox_can(&p_self->mailbox_pcu_rf_signal_send);
mailbox_pcu_send_failed:
  err--;
  gpio_read_destroy(&p_self->gpio_rtd_button);
read_gpio_rtd_button_failed:
  err--;
  gpio_read_destroy(&p_self->gpio_precharge_done);
read_gpio_precharge_done_failed:
  err--;
  gpio_read_destroy(&p_self->gpio_precharge_init);
read_gpio_precharge_init_failed:
  err--;
  car_speed_mut_destroy(&p_self->m_car_speed);
car_speed_init_failed:
  err--;
  hardware_init_can_destroy_ref_node(&p_self->can_inverter);
can_inverter_failed:
  err--;
  EmergencyNode_destroy(&p_self->amk_emergency);
EmergencyNode_failed:
  err--;

  return err;
}
