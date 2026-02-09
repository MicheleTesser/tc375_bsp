#include "torque_vec_alg.h"
#include "../../../../../core_utility/core_utility.h"
#include "../../../engines/engines.h"
#include "../../../../../../lib/DPS/dps.h"
#include "../../../../../../lib/tv_2025/ControlSystems.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include "../../../../../../lib/board_dbc/dbc/out_lib/can2/can2.h"
#pragma GCC diagnostic pop

#include <string.h>
#include <stdint.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include "../../../../../../lib/board_dbc/dbc/out_lib/can2/can2.h"
#pragma GCC diagnostic pop

#define NUM_OF_DPS 3

static struct{
  DpsSlave_h tv_dps[NUM_OF_DPS];
  struct CanMailbox* p_mailbox_send_dps_mex;
  SharedMessageReader_h m_shared_message_read_dps_master;
  FlashMemoryBank_h m_bank_var_csi;
  FlashMemoryBank_h m_bank_TEYM;
  struct CanMailbox* p_send_mailbox_cs_log_1;
  struct CanMailbox* p_send_mailbox_cs_log_2;
  struct CanMailbox* p_send_mailbox_cs_log_3;
  time_var_microseconds m_last_sent_cs_log;
}TV_ALG_DPS;

static int8_t _dps_send_fun(const DpsCanMessage* const restrict mex)
{
  return hardware_mailbox_send(TV_ALG_DPS.p_mailbox_send_dps_mex, mex->full_word);
}

static void _dps_wait_fun(void)
{
  timer_wait(get_tick_from_millis(100));
}

__attribute__((__unused__))
static int8_t _post_update_csi(const char* const name, const void* var)
{
  (void) name;
  uint32_t temp[2] = {0};
  memcpy(&temp, var, 4);
  return flash_memory_write_bank(&TV_ALG_DPS.m_bank_var_csi, temp);
}

__attribute__((__unused__))
static int8_t _post_update_TEYM(const char* const name, const void* var)
{
  (void) name;
  uint32_t temp[2] = {0};
  memcpy(&temp, var, 4);
  return flash_memory_write_bank(&TV_ALG_DPS.m_bank_TEYM, temp);
}

static void _log_var(const char* const name, void* const ptr )
{
  {
    LogEntry_h entry ={
      .data_format = FLOAT,
      .data_mode = __float__,
      .log_mode =  LOG_TELEMETRY | LOG_SD,
      .data_ptr = ptr,
    };
    strncpy(entry.name, name, sizeof(entry.name)-1);
    while (log_add_entry(&entry)<0)
    {
      serial_write_raw("log failed for: ");
      serial_write_str(name);
    }
  }
}

//public

void tv_alg_init(void)
{
  int8_t err=0;
  const char board_name_1[BOARD_NAME_LENGTH] = "VCUTV1";
  const char board_name_2[BOARD_NAME_LENGTH] = "VCUTV2";
  const char board_name_3[BOARD_NAME_LENGTH] = "VCUTV3";
  struct CanNode* p_node = NULL;
  union{
    uint32_t u32[2];
    float f32;
  }temp_buffer={0};
  const int32_t size = message_dlc(CAN_ID_DPSSLAVEMEX);

  while (!TV_ALG_DPS.p_send_mailbox_cs_log_1)
  {
    serial_write_str("init cs_log_1");
    ACTION_ON_CAN_NODE(CAN_GENERAL, p_node)
    {
      TV_ALG_DPS.p_send_mailbox_cs_log_1=
        hardware_get_mailbox_single_mex(
            p_node,
            SEND_MAILBOX,
            CAN_ID_CSLOG_1,
            (uint16_t)message_dlc_can2(CAN_ID_CSLOG_1));
    }
  }

  while (!TV_ALG_DPS.p_send_mailbox_cs_log_2)
  {
    serial_write_str("init cs_log_2");
    ACTION_ON_CAN_NODE(CAN_GENERAL, p_node)
    {
      TV_ALG_DPS.p_send_mailbox_cs_log_2=
        hardware_get_mailbox_single_mex(
            p_node,
            SEND_MAILBOX,
            CAN_ID_CSLOG_2,
            (uint16_t)message_dlc_can2(CAN_ID_CSLOG_2));
    }
  }

  while (!TV_ALG_DPS.p_send_mailbox_cs_log_3)
  {
    serial_write_str("init cs_log_3");
    ACTION_ON_CAN_NODE(CAN_GENERAL, p_node)
    {
      TV_ALG_DPS.p_send_mailbox_cs_log_3=
        hardware_get_mailbox_single_mex(
            p_node,
            SEND_MAILBOX,
            CAN_ID_CSLOG_3,
            (uint16_t)message_dlc_can2(CAN_ID_CSLOG_3));
    }
  }


  while ((err=shared_message_reader_init(&TV_ALG_DPS.m_shared_message_read_dps_master, SHARED_MEX_DPS_MASTER))<0)
  {
    timer_wait(get_tick_from_millis(200));
    serial_write_raw("err init shared_message_reader_init tv alg: ");
    serial_write_int8_t(err);
    serial_write_str("");
  }

  ControlSystems_ONLYFRONT_initialize();

  while ((err=flash_memory_req_bank(&TV_ALG_DPS.m_bank_var_csi, 3))<0)
  {
    timer_wait(get_tick_from_millis(200));
    serial_write_raw("err init bank var_csi tv alg: ");
    serial_write_int8_t(err);
    serial_write_str("");
  }

  flash_memory_read_bank(&TV_ALG_DPS.m_bank_var_csi,  temp_buffer.u32);
  ControlSystems_ONLYFRONT_P.variable_csi = temp_buffer.f32;

  while ((err=flash_memory_req_bank(&TV_ALG_DPS.m_bank_TEYM, 4))<0)
  {
    timer_wait(get_tick_from_millis(200));
    serial_write_raw("err init bank TEYM tv alg: ");
    serial_write_int8_t(err);
    serial_write_str("");
  }

  temp_buffer.u32[0]=0;
  temp_buffer.u32[1]=0;
  flash_memory_read_bank(&TV_ALG_DPS.m_bank_TEYM , temp_buffer.u32);
  ControlSystems_ONLYFRONT_P.TV_ENABLE_YR_MULT = temp_buffer.f32;
  while (!TV_ALG_DPS.p_mailbox_send_dps_mex)
  {
    serial_write_str("init mailbox tv alg send dps");
    ACTION_ON_CAN_NODE(CAN_GENERAL, p_node)
    {
      if (size >=0)
      {
        TV_ALG_DPS.p_mailbox_send_dps_mex = hardware_get_mailbox_single_mex(
            p_node,
            SEND_MAILBOX,
            CAN_ID_DPSSLAVEMEX, (uint16_t) size);
      }
      else
      {
        serial_write_raw("invalid size in: "__FILE__);
        serial_write_uint8_t(__LINE__);
        serial_write_raw(" size: ");
        serial_write_int32_t(size);
        serial_write_str("");
      }
    }
  }

  while((err=dps_slave_init(
        &TV_ALG_DPS.tv_dps[0],
        _dps_send_fun,
        _dps_wait_fun,
        board_name_1,
        1,
        CAN_ID_DPSMASTERMEX,
        CAN_ID_DPSSLAVEMEX))<0)
  {
    serial_write_raw("init dps slave tv 1 err: ");
    serial_write_int8_t(err);
    serial_write_str("");
  }


  while((err=dps_slave_init(
        &TV_ALG_DPS.tv_dps[1],
        _dps_send_fun,
        _dps_wait_fun,
        board_name_2,
        2,
        CAN_ID_DPSMASTERMEX,
        CAN_ID_DPSSLAVEMEX))<0)
  {
    serial_write_raw("init dps slave tv 2 err: ");
    serial_write_int8_t(err);
    serial_write_str("");
  }

  while((err=dps_slave_init(
        &TV_ALG_DPS.tv_dps[2],
        _dps_send_fun,
        _dps_wait_fun,
        board_name_3,
        3,
        CAN_ID_DPSMASTERMEX,
        CAN_ID_DPSSLAVEMEX))<0)
  {
    serial_write_raw("init dps slave tv 3 err: ");
    serial_write_int8_t(err);
    serial_write_str("");
  }

  for (uint8_t i=0; i<NUM_OF_DPS; i++)
  {
    dps_slave_start(&TV_ALG_DPS.tv_dps[i]);
  }

#define PUSH_TV_PARAM(DPS,PARAM, NAME, UPDATE_F) \
  const char __custom_param_##NAME [VAR_NAME_LENGTH] = #NAME;\
  if((err=dps_monitor_primitive_var(\
      &TV_ALG_DPS.DPS,\
      DPS_TYPES_FLOAT_T,\
      &ControlSystems_ONLYFRONT_P.PARAM,\
      UPDATE_F,\
      __custom_param_##NAME))<0)\
  {\
    serial_write_str("err monitiring "#NAME);\
  }

  PUSH_TV_PARAM(tv_dps[0], ABS_P_F, ASPF, NULL);
  PUSH_TV_PARAM(tv_dps[0], ABS_P_R, ASPR, NULL);

  PUSH_TV_PARAM(tv_dps[0], ABS_SCALE, ASCL, NULL);
  PUSH_TV_PARAM(tv_dps[0], ABS_TARGET_SLIP_F, ATSF, NULL);

  PUSH_TV_PARAM(tv_dps[0], ABS_TARGET_SLIP_R, ATSR, NULL);
  PUSH_TV_PARAM(tv_dps[0], IN_ACC_X_LP_HZ, IACX, NULL);

  PUSH_TV_PARAM(tv_dps[0], IN_ACC_Y_LP_HZ, IACY, NULL);
  PUSH_TV_PARAM(tv_dps[0], IN_BRAKE_MAP_EXP, IBRK, NULL);

  PUSH_TV_PARAM(tv_dps[0], IN_STEERING_LP_HZ, ISTW, NULL);
  PUSH_TV_PARAM(tv_dps[0], IN_THROTTLE_MAP_EXP, ITHR, NULL);

  PUSH_TV_PARAM(tv_dps[0], IN_YAW_RATE_LP_HZ, IWRL, NULL);
  PUSH_TV_PARAM(tv_dps[0], Imax_regen, Imre, NULL);

  PUSH_TV_PARAM(tv_dps[0], Pmax, Pmax, NULL);
  PUSH_TV_PARAM(tv_dps[0], REGEN_TYPE, RegT, NULL);

  PUSH_TV_PARAM(tv_dps[0], TC_CL_I_F, TLIF, NULL);
  PUSH_TV_PARAM(tv_dps[1], TC_CL_I_R, TLIR, NULL);

  PUSH_TV_PARAM(tv_dps[1], TC_CL_P_F, TLPF, NULL);
  PUSH_TV_PARAM(tv_dps[1], TC_CL_P_R, TLPR, NULL);

  PUSH_TV_PARAM(tv_dps[1], TC_CL_TARGET_SLIP_F, TLSF, NULL);
  PUSH_TV_PARAM(tv_dps[1], TC_CL_TARGET_SLIP_R, TLSR, NULL);

  PUSH_TV_PARAM(tv_dps[1], TC_SCALE, TSCA, NULL);
  PUSH_TV_PARAM(tv_dps[1], TC_STRAT, TSTR, NULL);

  PUSH_TV_PARAM(tv_dps[1], TV_ENABLE_YR_MULT, TEYM, NULL);
  PUSH_TV_PARAM(tv_dps[1], TV_HIGH_SPEED_THRESHOLD, THST, NULL);

  PUSH_TV_PARAM(tv_dps[1], TV_YR_HIGH_SPEED_MULT, THSM, NULL);
  PUSH_TV_PARAM(tv_dps[1], TV_YR_LOW_SPEED_MULT, TLSM, NULL);

  PUSH_TV_PARAM(tv_dps[1], Tmax, Tmax, NULL);
  PUSH_TV_PARAM(tv_dps[1], Tmax_regen, TmRg, NULL);

  PUSH_TV_PARAM(tv_dps[1], YMC_TRQ_DELTA_LIMIT, DTLI, NULL);
  PUSH_TV_PARAM(tv_dps[1], YMC_TRQ_DELTA_LIMIT_COASTING, DTLC, NULL);
  PUSH_TV_PARAM(tv_dps[1], csi, csi, NULL);

  PUSH_TV_PARAM(tv_dps[2], variable_csi, vcsi_, NULL);

  _log_var("CS_yaw_moment", &ControlSystems_ONLYFRONT_Y.yaw_moment);
  _log_var("CS_yr_ref", &ControlSystems_ONLYFRONT_Y.yr_ref);
  _log_var("CS_vx", &ControlSystems_ONLYFRONT_Y.vx);
  _log_var("CS_delta_torque_fl", &ControlSystems_ONLYFRONT_Y.delta_torque[FRONT_LEFT]);
  _log_var("CS_delta_torque_fr", &ControlSystems_ONLYFRONT_Y.delta_torque[FRONT_RIGHT]);
  _log_var("CS_delta_torque_rl", &ControlSystems_ONLYFRONT_Y.delta_torque[REAR_LEFT]);
  _log_var("CS_delta_torque_rr", &ControlSystems_ONLYFRONT_Y.delta_torque[REAR_RIGHT]);

}

void tv_alg_update(void)
{
  uint64_t data=0;
  can_obj_can2_h_t o2 = {0};

  if (shared_message_read(&TV_ALG_DPS.m_shared_message_read_dps_master, &data))
  {
    DpsCanMessage dps_mex = 
    {
      .dlc = (uint8_t) message_dlc(CAN_ID_DPSMASTERMEX),
      .full_word = data,
      .id = CAN_ID_DPSMASTERMEX,
    };
    for (uint8_t i=0; i<NUM_OF_DPS; ++i)
    {
      (void)dps_slave_check_can_command_recv(&TV_ALG_DPS.tv_dps[i], &dps_mex);
    }
  }

  ACTION_ON_FREQUENCY(TV_ALG_DPS.m_last_sent_cs_log, get_tick_from_millis(5))
  {
    uint64_t payload_cs_1 =0;
    uint64_t payload_cs_2 =0;
    uint64_t payload_cs_3 =0;
    float temp =0;

    //INFO: log_1
    temp = (float) ControlSystems_ONLYFRONT_Y.yaw_moment;
    memcpy(&o2.can_0x334_CsLog_1.yaw_moment, &temp , 4);
    temp = (float) ControlSystems_ONLYFRONT_Y.yr_ref;
    memcpy(&o2.can_0x334_CsLog_1.yr_ref , &temp, 4);

#define TRUNC(num) (int16_t) (num * 1000)

    //INFO: log_2
    o2.can_0x335_CsLog_2.delta_torque_fl = TRUNC(ControlSystems_ONLYFRONT_Y.delta_torque[FRONT_LEFT]);
    o2.can_0x335_CsLog_2.delta_torque_fr = TRUNC(ControlSystems_ONLYFRONT_Y.delta_torque[FRONT_RIGHT]);
    o2.can_0x335_CsLog_2.delta_torque_rl = TRUNC(ControlSystems_ONLYFRONT_Y.delta_torque[REAR_LEFT]);
    o2.can_0x335_CsLog_2.delta_torque_rr = TRUNC(ControlSystems_ONLYFRONT_Y.delta_torque[REAR_RIGHT]);

    temp = (float) ControlSystems_ONLYFRONT_Y.vx;

    //INFO: log_3
    memcpy(&o2.can_0x336_CsLog_3.vx, &temp, 4);

#define SEND_CS(MAILBOX,ID, PAYLOAD)\
    if (pack_message_can2(&o2, ID, &PAYLOAD)<0)\
    {\
      SET_TRACE(CORE_0, PACK_ERROR);\
    }\
    else\
    {\
      hardware_mailbox_send(TV_ALG_DPS.MAILBOX, PAYLOAD);\
    }

    SEND_CS(p_send_mailbox_cs_log_1, CAN_ID_CSLOG_1, payload_cs_1);
    SEND_CS(p_send_mailbox_cs_log_2, CAN_ID_CSLOG_2, payload_cs_2);
    SEND_CS(p_send_mailbox_cs_log_3, CAN_ID_CSLOG_3, payload_cs_3);
  }
}

void tv_alg_compute(const struct TVInputArgs* const restrict input,
        float o_posTorquesNM[__NUM_OF_ENGINES__])
{
  ControlSystems_ONLYFRONT_P.csi = input->power_repartition;
  ControlSystems_ONLYFRONT_P.Pmax = input->limit_power;
  ControlSystems_ONLYFRONT_P.Tmax = input->torque_max;
  ControlSystems_ONLYFRONT_P.Tmax_regen= input->torque_regen_max;
  ControlSystems_ONLYFRONT_P.Imax_regen = input->regen_max_current;
  ControlSystems_ONLYFRONT_P.TC_STRAT = input->TC_strat;
  ControlSystems_ONLYFRONT_P.REGEN_TYPE = 0;
  ControlSystems_ONLYFRONT_P.Pe_lim = input->limit_power + 5000;
  ControlSystems_ONLYFRONT_P.Vcell_lim = 3.0;

  ControlSystems_ONLYFRONT_U.brake= input->brake;
  ControlSystems_ONLYFRONT_U.throttle = input->throttle;
  ControlSystems_ONLYFRONT_U.battery_current= input->current;
  ControlSystems_ONLYFRONT_U.battery_voltage= input->voltage;
  ControlSystems_ONLYFRONT_U.steering_g = input->steering_angle;
  ControlSystems_ONLYFRONT_U.acc_x = input->acc_x;
  ControlSystems_ONLYFRONT_U.acc_y = input->acc_y;
  ControlSystems_ONLYFRONT_U.yaw_rate = input->yaw_rate;
  ControlSystems_ONLYFRONT_U.TV_active = input->TV_active;
  ControlSystems_ONLYFRONT_U.Regen_active = input->REGEN_active;
  ControlSystems_ONLYFRONT_U.Vcell_min = input->Vcell_min;
  ControlSystems_ONLYFRONT_U.rpm_fl = input->rpm[FRONT_LEFT];
  ControlSystems_ONLYFRONT_U.rpm_fr = input->rpm[FRONT_RIGHT];
  ControlSystems_ONLYFRONT_U.rpm_rl = input->rpm[REAR_LEFT];
  ControlSystems_ONLYFRONT_U.rpm_rr = input->rpm[REAR_RIGHT];
  ControlSystems_ONLYFRONT_U.LC_button = 0;


  ControlSystems_ONLYFRONT_step();

  o_posTorquesNM[FRONT_LEFT] = (float) ControlSystems_ONLYFRONT_Y.torques[FRONT_LEFT];
  o_posTorquesNM[FRONT_RIGHT] = (float) ControlSystems_ONLYFRONT_Y.torques[FRONT_RIGHT];
  o_posTorquesNM[REAR_LEFT] = (float) ControlSystems_ONLYFRONT_Y.torques[REAR_LEFT];
  o_posTorquesNM[REAR_RIGHT] = (float) ControlSystems_ONLYFRONT_Y.torques[REAR_RIGHT];
}
