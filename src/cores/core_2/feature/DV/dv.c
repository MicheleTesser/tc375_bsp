#include "dv.h"
#include "res/res.h"
#include "ebs/ebs.h"
#include "../../../../lib/raceup_board/raceup_board.h"
#include "../../../core_utility/core_utility.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include "../../../../lib/board_dbc/dbc/out_lib/can3/can3.h"
#include "../../../../lib/board_dbc/dbc/out_lib/can2/can2.h"
#pragma GCC diagnostic pop 

#include <stdint.h>
#include <string.h>

//private

#define ERR_TRACE(ERR)\
  {\
    SET_TRACE(CORE_2,ERR);\
    err--;\
  }

#define NS_AS(name) NS_DV(AS_##name)
enum NS_AS(){
  NS_AS(OFF) =1,
  NS_AS(READY),
  NS_AS(DRIVING),
  NS_AS(EMERGENCY),
  NS_AS(FINISHED),

  NS_AS(__NUM_OF__)
};

#define NS_CS(name) NS_DV(CAR_STATUS_##name)
enum NS_CS(){
  NS_CS(OFF)=0,
  NS_CS(READY),
  NS_CS(ERROR),

  NS_CS(__NUM_OF__)
};

//INFO: src: can3.dbc message id 126 field Embedded_status
//1 "off"
//2 "ready"
//3 "running"
//4 "error"
//5 "finished"
#define NS_ES(name) NS_DV(DV_EMBEDDED_STATUS_##name)
enum NS_ES(){
  NS_ES(OFF) = 1,
  NS_ES(READY),
  NS_ES(RUNNING),
  NS_ES(ERROR),
  NS_ES(FINISHED),
};

//INFO: copied from .dbc can2 CarMissionStatus
#define NS_MS(name) NS_DV(MISSION_STATUS_##name)
enum NS_MS(){
  NS_MS(NOT_RUNNING)=0,
  NS_MS(RUNNING),
  NS_MS(FINISHED),
};

#define NS_DE(name) NS_DV(EMERENGENCY_##name)
enum NS_DE(){
  NS_DE(EMERGENCY)=0,
  NS_DE(EMBEDDED_OFF),
  NS_DE(RES_ERR),
  NS_DE(RES_OFF),
  NS_DE(EBS_ERR),
  NS_DE(BRAKE_PRESSURE),
  NS_DE(ERROR_MISSION_STATUS_EMBEDDED),
  NS_DE(INITIAL_CHECKUP_SDC_CLOSED),
  NS_DE(UNREACHABLE_CODE),

  NS_DE(__NUM_OF__)
};

struct Dv_t{
  enum NS_AS() m_status;
  enum NS_ES() m_dv_embedded_status;
  enum NS_MS() m_dv_mission_status;
  uint8_t m_ever_been_as_ready:1;
  uint8_t m_sound_start:1;
  uint8_t m_recv_asms:1;
  uint8_t m_as_node_closed:1;
  time_var_microseconds m_driving_last_time_on; 
  EmergencyNode_h m_emergency_node;
  time_var_microseconds m_emergency_last_time_on; 
  time_var_microseconds m_embeed_last_alive; 
  time_var_microseconds m_start_exintg_continuous_monitoring; 
  time_var_microseconds m_last_mission_status; 
  time_var_microseconds m_last_sent_pcu; 
  time_var_microseconds m_last_assi_ligh; 
  time_var_microseconds m_last_initial_checkup; 
  DvRes_h m_dv_res; 
  DvEbs_h m_dv_ebs;
  AsNodeMut_h m_as_node;
  GpioRead_h m_gpio_air_precharge_init; 
  GpioRead_h m_gpio_air_precharge_done; 
  RtdAssiSound_h m_assi_sound;
  Gpio_h m_gpio_ass_light_blue; 
  Gpio_h m_gpio_ass_light_yellow; 
  Imu_h m_imu; 
  MissionLocker_h m_mission_locker;
  CarMissionReader_h* p_mission_reader; 
  SharedMessageReader_h m_recv_mission_status; 
  struct CanMailbox* p_send_car_m_car_status_mailbox; 
  struct CanMailbox* p_send_pcu_dv; 
  struct CanMailbox* p_mailbox_recv_asms; 
};


union Dv_h_t_conv{
  Dv_h* const hidden;
  struct Dv_t* const clear;
};

#ifdef DEBUG
char __assert_size_dv[(sizeof(Dv_h) == sizeof(struct Dv_t))? 1:-1];
char __assert_align_dv[(_Alignof(Dv_h) == _Alignof(struct Dv_t))? 1:-1];
#endif // DEBUG

//INFO: check dbc of can3 in message DV_system_status

static inline uint8_t _sdc_closed(const struct Dv_t* const restrict self)
{
  return  !gpio_read_state(&self->m_gpio_air_precharge_init) && as_node_read_get();
}

/*
 * AS Off -> off
 * AS Ready -> yellow continuous
 * AS Driving -> yellow flashing (100 MILLIS)
 * AS Emergency -> blue flashing (100 MILLIS)
 * AS Finished -> blue continuous
 */
static int8_t _dv_update_led(struct Dv_t* const restrict self)
{
  Gpio_h* const restrict gpio_light_blue = &self->m_gpio_ass_light_blue;
  Gpio_h* const restrict gpio_light_yellow = &self->m_gpio_ass_light_yellow;

  switch (self->m_status)
  {
    case NS_AS(OFF):
      gpio_set_low(gpio_light_blue);
      gpio_set_low(gpio_light_yellow);
      NS_RES(reset_start_time_go)(&self->m_dv_res);
      break;
    case NS_AS(READY):
      gpio_set_low(gpio_light_blue);
      gpio_set_high(gpio_light_yellow);
      NS_RES(start_time_go)(&self->m_dv_res);
      break;
    case NS_AS(DRIVING):
      ACTION_ON_FREQUENCY(self->m_last_assi_ligh, get_tick_from_millis(500))
      {
        gpio_toggle(gpio_light_yellow);
      }
      gpio_set_low(gpio_light_blue);
      break;
    case NS_AS(EMERGENCY):
      if (self->m_ever_been_as_ready)
      {
        gpio_set_low(gpio_light_yellow);
        ACTION_ON_FREQUENCY(self->m_last_assi_ligh, get_tick_from_millis(125))
        {
          gpio_toggle(gpio_light_blue);
        }
        NS_RES(reset_start_time_go)(&self->m_dv_res);
        if (!self->m_sound_start)
        {
          self->m_sound_start =1;
          NS_ASSIS_SOUND(start)(&self->m_assi_sound);
        }
      }
      break;
    case NS_AS(FINISHED):
      gpio_set_high(gpio_light_blue);
      gpio_set_low(gpio_light_yellow);
      NS_RES(reset_start_time_go)(&self->m_dv_res);
      break;
    case NS_AS(__NUM_OF__):
    default:
      EmergencyNode_raise(&self->m_emergency_node, NS_DE(UNREACHABLE_CODE));
      NS_RES(reset_start_time_go)(&self->m_dv_res);
      SET_TRACE(CORE_2, UNREACHABLE_CODE);
      return -1;

  }

  return 0;
}

static uint8_t _check_embedded(struct Dv_t* const p_self)
{
  return (timer_time_now() - p_self->m_embeed_last_alive) < get_tick_from_millis(200);
}

static int8_t _dv_initial_checkup(struct Dv_t* const restrict p_self)
{
  int8_t err=0;

  if (!NS_RES(started)(&p_self->m_dv_res))
  {
    SET_TRACE(CORE_2, DV_INITIAL_CHECKUP);
    return --err;
  }

  if (!NS_RES(check_alive)(&p_self->m_dv_res))
  {
    ERR_TRACE(DV_INITIAL_CHECKUP);
    EmergencyNode_raise(&p_self->m_emergency_node, NS_DE(RES_OFF));
    return --err;
  }
  else
  {
    EmergencyNode_solve(&p_self->m_emergency_node, NS_DE(RES_OFF));
  }

  if (!p_self->m_embeed_last_alive)
  {
    SET_TRACE(CORE_2, DV_INITIAL_CHECKUP);
    return --err;
  }

  if (!_check_embedded(p_self))

  {
    ERR_TRACE(DV_INITIAL_CHECKUP);
    EmergencyNode_raise(&p_self->m_emergency_node, NS_DE(EMBEDDED_OFF));
    return --err;
  } else {
    EmergencyNode_solve(&p_self->m_emergency_node, NS_DE(EMBEDDED_OFF));
  }


  if (p_self->m_dv_embedded_status != NS_ES(READY))
  {
    SET_TRACE(CORE_2, DV_INITIAL_CHECKUP);
    return --err;
  }

  if (!ebs_ok(&p_self->m_dv_ebs))
  {
    ERR_TRACE(DV_INITIAL_CHECKUP);
    EmergencyNode_raise(&p_self->m_emergency_node, NS_DE(BRAKE_PRESSURE));
    return --err;
  }
  else
  {
    EmergencyNode_solve(&p_self->m_emergency_node, NS_DE(BRAKE_PRESSURE));
  }

  as_node_mut_close(&p_self->m_as_node);
  p_self->m_as_node_closed = 1;

  if (global_running_status_get() < TS_READY)
  {
    SET_TRACE(CORE_2, DV_INITIAL_CHECKUP);
    return --err;
  }

  if (ebs_asb_consistency_check(&p_self->m_dv_ebs) != EBS_OK)
  {
    SET_TRACE(CORE_2, DV_INITIAL_CHECKUP);
    return --err;
  }
  
  p_self->m_ever_been_as_ready = 1;
  p_self->m_status = NS_AS(READY);

  return 0;

}

static int8_t _dv_exiting_continuos_monitoring(struct Dv_t* const restrict p_self)__attribute__((__unused__));
static int8_t _dv_exiting_continuos_monitoring(struct Dv_t* const restrict p_self)
{
  int8_t err=0;

  if (!p_self->m_start_exintg_continuous_monitoring)
  {
    p_self->m_start_exintg_continuous_monitoring = timer_time_now();
  }

  if ((timer_time_now() - p_self->m_start_exintg_continuous_monitoring) > get_tick_from_millis(500))
  {
    if (ebs_brake_consistent(&p_self->m_dv_ebs) == EBS_NO)
    {
      EmergencyNode_raise(&p_self->m_emergency_node, NS_DE(BRAKE_PRESSURE));
      ERR_TRACE(DV_CONTINUOUS_CHECKUP);
      return --err;
    }

    if (NS_SH(get)(NS_SD(GO_STATUS))==NS_RT(ERROR))
    {
      EmergencyNode_raise(&p_self->m_emergency_node, NS_DE(RES_ERR));
      ERR_TRACE(DV_CONTINUOUS_CHECKUP);
      return --err;
    }

    if (p_self->m_dv_mission_status == NS_MS(FINISHED)) //HACK: && car_speed_get() <= 2.0f)
    {
      p_self->m_status = NS_AS(FINISHED);
    }
    else
    {
      EmergencyNode_raise(&p_self->m_emergency_node, NS_DE(EBS_ERR));
      ERR_TRACE(DV_CONTINUOUS_CHECKUP);
      return --err;
    }
  }

  return 0;
}

static int8_t _dv_continuos_monitoring(struct Dv_t* const restrict p_self)__attribute__((__unused__));
static int8_t _dv_continuos_monitoring(struct Dv_t* const restrict p_self)
{
  int8_t err=0;

  if (!_check_embedded(p_self))
  {
    EmergencyNode_raise(&p_self->m_emergency_node, NS_DE(EMBEDDED_OFF));
    p_self->m_status = NS_AS(EMERGENCY);
    ERR_TRACE(DV_CONTINUOUS_CHECKUP);
    return --err;
  }

  // if (!NS_RES(check_alive)(&p_self->m_dv_res)) HACK:
  // {
  //   ERR_TRACE(DV_CONTINUOUS_CHECKUP);
  //   EmergencyNode_raise(&p_self->m_emergency_node, NS_DE(RES_OFF));
  //   return --err;
  // }
  // else
  // {
  //   EmergencyNode_solve(&p_self->m_emergency_node, NS_DE(RES_OFF));
  // }

  if (!ebs_on(&p_self->m_dv_ebs))
  {
    EmergencyNode_raise(&p_self->m_emergency_node, NS_DE(EBS_ERR));
    ERR_TRACE(DV_CONTINUOUS_CHECKUP);
    return --err;
  }

  if (!ebs_ok(&p_self->m_dv_ebs))
  {
    EmergencyNode_raise(&p_self->m_emergency_node, NS_DE(BRAKE_PRESSURE));
    ERR_TRACE(DV_CONTINUOUS_CHECKUP);
    return --err;
  }

  return 0;
}

static int8_t _dv_mission_update(struct Dv_t* const restrict p_self)
{
  can_obj_can3_h_t o3={0};
  can_obj_can2_h_t o2={0};
  uint64_t mex=0;
  int8_t err=0;


  ACTION_ON_FREQUENCY(p_self->m_last_sent_pcu, get_tick_from_millis(100))
  {
    o2.can_0x130_Pcu.mode = 2;
    o2.can_0x130_Pcu.enable_dv =1;
    o2.can_0x130_Pcu.enable_embedded =1;

    if(pack_message_can2(&o2, CAN_ID_PCU, &mex)<0)
    {
      SET_TRACE(CORE_2, PACK_ERROR);
    }

    if (hardware_mailbox_send(p_self->p_send_pcu_dv, mex)<0)
    {
      SET_TRACE(CORE_2, CAN_SEND);
    }
  }


  switch (NS_SH(get)(NS_SD(GO_STATUS)))
  {
    case NS_RT(OFF):
      EmergencyNode_solve(&p_self->m_emergency_node, NS_DE(RES_ERR));
      break;
    case NS_RT(DRIVE):
      break;
    case NS_RT(ERROR):
    default:
      SET_TRACE(CORE_2, DV_EMERGENCY);
     // EmergencyNode_raise(&p_self->m_emergency_node, NS_DE(RES_ERR));
      p_self->m_status=NS_AS(EMERGENCY);
      
      as_node_mut_open(&p_self->m_as_node);
      return --err;
  }

  if (shared_message_read_unpack_can3(&p_self->m_recv_mission_status, &o3)>0)
  {
    EmergencyNode_solve(&p_self->m_emergency_node, NS_DE(EMBEDDED_OFF));
    p_self->m_dv_embedded_status = o3.can_0x07e_DV_EMBEDDED_Status.Embedded_status;
    p_self->m_embeed_last_alive = timer_time_now(); 
    switch (p_self->m_dv_embedded_status)
    {
      case NS_ES(OFF):
        p_self->m_dv_mission_status = NS_MS(NOT_RUNNING);
        EmergencyNode_solve(&p_self->m_emergency_node, NS_DE(ERROR_MISSION_STATUS_EMBEDDED));
        break;
      case NS_ES(READY):
        p_self->m_dv_mission_status = NS_MS(NOT_RUNNING);
        break;
      case NS_ES(RUNNING):
        p_self->m_dv_mission_status = NS_MS(RUNNING);
        break;
      case NS_ES(ERROR):
        p_self->m_dv_mission_status = NS_MS(FINISHED);
        p_self->m_status = NS_AS(EMERGENCY);
        ERR_TRACE(DV_EMERGENCY);
        //EmergencyNode_raise(&p_self->m_emergency_node, NS_DE(ERROR_MISSION_STATUS_EMBEDDED)); 
        as_node_mut_open(&p_self->m_as_node);
        return --err;
        break;
      case NS_ES(FINISHED):
        p_self->m_dv_mission_status = NS_MS(FINISHED);
        as_node_mut_open(&p_self->m_as_node);
        break;
    }
  }


  if (p_self->m_status == NS_AS(OFF))  //INFO: initial checkup
  {
    ACTION_ON_FREQUENCY(p_self->m_last_initial_checkup, get_tick_from_millis(2))
    {
      err += _dv_initial_checkup(p_self);
    }
  }

  if (p_self->m_status == NS_AS(READY) || p_self->m_status == NS_AS(DRIVING)) //INFO: continuous monitoring
  {
    switch (global_running_status_get())
    {
      case SYSTEM_OFF:
      case SYSTEM_PRECAHRGE:
        EmergencyNode_raise(&p_self->m_emergency_node, NS_DE(UNREACHABLE_CODE));
        p_self->m_status = NS_AS(EMERGENCY);
        ERR_TRACE(UNREACHABLE_CODE);
        break;
      case TS_READY:
        p_self->m_status = NS_AS(READY);
        break;
      case RUNNING:
        p_self->m_status = NS_AS(DRIVING);
        break;
    }
    if (!_sdc_closed(p_self))
    {
      err += _dv_exiting_continuos_monitoring(p_self);
    }
    else
    {
      err += _dv_continuos_monitoring(p_self);
    }
  }

  return err;
}

//public

int8_t NS_DV(init)(Dv_h* const restrict self ,
    CarMissionReader_h* const restrict p_mission_reader)
{
  union Dv_h_t_conv conv = {self};
  struct Dv_t* const p_self = conv.clear;
  struct CanNode* can_node;
  int8_t err=0;

  memset(p_self, 0, sizeof(*p_self));

  if(NS_RES(init)(&p_self->m_dv_res)<0)
  {
    goto res_failed;
  }

  if(ebs_class_init(&p_self->m_dv_ebs) <0)
  {
    goto ebs_failed;
  }

  if (imu_init(&p_self->m_imu)<0)
  {
    goto imu_failed;
  }

  if (lock_mission_ref_get_mut(&p_self->m_mission_locker)<0)
  {
    goto lock_mission_ref_failed;
  }

  if (hardware_init_gpio(&p_self->m_gpio_ass_light_yellow, GPIO_ASSI_LIGHT_YELLOW, 1)<0)
  {
    goto gpio_assi_yellow_failed;
  }

  if (hardware_init_gpio(&p_self->m_gpio_ass_light_blue, GPIO_ASSI_LIGHT_BLUE, 1)<0)
  {
    goto gpio_assi_blue_failed;
  }


  AssiMetadata data =
  {
    .duration = get_tick_from_millis(8000),
    .duty_cycle_min = 60,
    .duty_cycle_max = 100,
    .duty_cycle_step = get_tick_from_millis(1500),
  };
  if(NS_ASSIS_SOUND(init)(&p_self->m_assi_sound,&data)<0)
  {
    goto assi_sound_init_failed;
  }


  if (hardware_init_read_permission_gpio(&p_self->m_gpio_air_precharge_init,
        GPIO_AIR_PRECHARGE_INIT)<0)
  {
    goto gpio_precharge_init_failed;
  }

  if (hardware_init_read_permission_gpio(&p_self->m_gpio_air_precharge_done,
        GPIO_AIR_PRECHARGE_DONE)<0)
  {
    goto gpio_precharge_done_failed;
  }

  if (EmergencyNode_init(&p_self->m_emergency_node)<0)
  {
    goto emergencey_node_failed;
  }

  if (as_node_mut_init(&p_self->m_as_node)<0)
  {
    goto as_node_failed;
  }


  ACTION_ON_CAN_NODE(CAN_GENERAL,can_node)
  {
    p_self->p_send_car_m_car_status_mailbox =
    hardware_get_mailbox_single_mex(
        can_node,
        SEND_MAILBOX,
        CAN_ID_CARMISSIONSTATUS,
        (uint16_t) message_dlc_can2(CAN_ID_CARMISSIONSTATUS));
  }
  if (!p_self->p_send_car_m_car_status_mailbox)
  {
    goto mailbox_car_mission_status_failed;
  }

  
  if (shared_message_reader_init(&p_self->m_recv_mission_status, SHARED_MEX_DV_EMBEDDED_STATUS))
  {
    goto shared_mex_init_failed;
  }

  ACTION_ON_CAN_NODE(CAN_GENERAL, can_node)
  {
    p_self->p_mailbox_recv_asms=
    hardware_get_mailbox_single_mex(
        can_node,
        RECV_MAILBOX,
        CAN_ID_ASMS,
        (uint16_t) message_dlc_can2(CAN_ID_ASMS));
  }


  if (!p_self->p_mailbox_recv_asms)
  {
    goto mailbox_asms_failed;
  }

  ACTION_ON_CAN_NODE(CAN_GENERAL, can_node)
  {
    p_self->p_send_pcu_dv=
    hardware_get_mailbox_single_mex(
        can_node,
        SEND_MAILBOX,
        CAN_ID_PCU,
        (uint16_t) message_dlc_can2(CAN_ID_PCU));
  }


  if (!p_self->p_send_pcu_dv)
  {
    goto mailbox_pcu_failed;
  }

  p_self->m_dv_mission_status = NS_MS(NOT_RUNNING);
  p_self->p_mission_reader = p_mission_reader;
  p_self->m_dv_embedded_status = NS_ES(OFF);
  p_self->m_status = NS_AS(OFF);
  p_self->m_embeed_last_alive = 0;
  p_self->m_ever_been_as_ready = 0;
  return 0;


  err--;
  hardware_free_mailbox_can(&p_self->p_send_pcu_dv);
mailbox_pcu_failed:
  err--;
  hardware_free_mailbox_can(&p_self->p_mailbox_recv_asms);
mailbox_asms_failed:
  err--;
  shared_message_read_destroy(&p_self->m_recv_mission_status);
shared_mex_init_failed:
  err--;
  hardware_free_mailbox_can(&p_self->p_send_car_m_car_status_mailbox);
mailbox_car_mission_status_failed:
  err--;
  as_node_mut_destroy(&p_self->m_as_node);
as_node_failed:
  err--;
  EmergencyNode_destroy(&p_self->m_emergency_node);
emergencey_node_failed:
  err--;
  gpio_read_destroy(&p_self->m_gpio_air_precharge_done);
gpio_precharge_done_failed:
  err--;
  gpio_read_destroy(&p_self->m_gpio_air_precharge_init);
gpio_precharge_init_failed:
  err--;
  NS_ASSIS_SOUND(destroy)(&p_self->m_assi_sound);
assi_sound_init_failed:
  err--;
  gpio_destroy(&p_self->m_gpio_ass_light_blue);
gpio_assi_blue_failed:
  err--;
  gpio_destroy(&p_self->m_gpio_ass_light_yellow);
gpio_assi_yellow_failed:
  err--;
  lock_mission_ref_destroy_mut(&p_self->m_mission_locker);
lock_mission_ref_failed:
  err--;
  imu_destroy(&p_self->m_imu);
imu_failed:
  err--;
  ebs_destroy(&p_self->m_dv_ebs);
ebs_failed:
  err--;
  NS_RES(destroy)(&p_self->m_dv_res);
res_failed:
  err--;

  return err;
}

int8_t NS_DV(update)(Dv_h* const restrict self)
{
  union Dv_h_t_conv conv = {self};
  struct Dv_t* const p_self = conv.clear;
  uint64_t mission_status_payload = 0;
  can_obj_can2_h_t o2 = {0};
  int8_t err=0;
  CanMessage mex = {0};

  if (ebs_update(&p_self->m_dv_ebs)<0) ERR_TRACE(INIT_ERROR);
  if (imu_update(&p_self->m_imu)<0) ERR_TRACE(INIT_ERROR);
  // if (NS_RES(update)(&p_self->m_dv_res)<0) ERR_TRACE(INIT_ERROR);
  
  if (hardware_mailbox_read(p_self->p_mailbox_recv_asms, &mex))
  {
    if(unpack_message_can2(&o2, mex.id, mex.full_word, mex.message_size, (dbcc_time_stamp_t) timer_time_now())<0)
    {
      ERR_TRACE(UNPACK_ERROR);
    }
    else
    {
      p_self->m_recv_asms = o2.can_0x041_Asms.Asms_sens;
      if (o2.can_0x041_Asms.Asms_sens)
      {
        lock_mission(&p_self->m_mission_locker);
      }
      else
      {
        unlock_mission(&p_self->m_mission_locker);
      }
    }
  }

  switch (NS_CAR_MISS(get_current_mission)(p_self->p_mission_reader))
  {
    case NS_CAR_MISS(NONE):
      as_node_mut_open(&p_self->m_as_node);
      p_self->m_status = NS_AS(OFF);
      p_self->m_dv_mission_status = NS_MS(NOT_RUNNING);
      break;
    case NS_CAR_MISS(HUMAN):
      p_self->m_dv_mission_status = NS_MS(RUNNING);
      p_self->m_status = NS_AS(OFF);
      if (!_check_embedded(p_self) ||
          !ebs_on(&p_self->m_dv_ebs) ||
          !NS_RES(started)(&p_self->m_dv_res) ||
          !p_self->m_recv_asms)
      {
        p_self->m_as_node_closed =0;
        as_node_mut_close(&p_self->m_as_node);
      }
      break;
    case NS_CAR_MISS(DV_ACCELERATION):
    case NS_CAR_MISS(DV_SKIDPAD):
    case NS_CAR_MISS(DV_AUTOCROSS):
    case NS_CAR_MISS(DV_TRACKDRIVE):
    case NS_CAR_MISS(DV_EBS_TEST):
    case NS_CAR_MISS(DV_INSPECTION):
      if (!p_self->m_as_node_closed)
      {
        as_node_mut_open(&p_self->m_as_node);
      }
      err += _dv_mission_update(p_self);
      break;
    case NS_CAR_MISS(__NUM_OF__):
      p_self->m_dv_mission_status = NS_MS(NOT_RUNNING);
      p_self->m_status = NS_AS(OFF);
      EmergencyNode_raise(&p_self->m_emergency_node, NS_DE(UNREACHABLE_CODE));
      ERR_TRACE(UNREACHABLE_CODE);
      break;
  }

  if(_dv_update_led(p_self)<0)
  {
    ERR_TRACE(UPDATE_ERR);
  }


  ACTION_ON_FREQUENCY(p_self->m_last_mission_status, get_tick_from_millis(50))
  {
    o2.can_0x071_CarMissionStatus.Mission = (uint8_t) NS_CAR_MISS(get_current_mission)(p_self->p_mission_reader);
    o2.can_0x071_CarMissionStatus.MissionStatus = (uint8_t) p_self->m_dv_mission_status;
    o2.can_0x071_CarMissionStatus.AsStatus = (uint8_t) p_self->m_status;

    if(pack_message_can2(&o2, CAN_ID_CARMISSIONSTATUS, &mission_status_payload)<0)
    {
      ERR_TRACE(PACK_ERROR);
    } else if(hardware_mailbox_send(p_self->p_send_car_m_car_status_mailbox, mission_status_payload)<0)
    {
      ERR_TRACE(CAN_SEND);
    }
  }

  return err;
}

int8_t NS_DV(destroy)(Dv_h* const restrict self)
{
  union Dv_h_t_conv conv = {self};
  struct Dv_t* const p_self = conv.clear;

  hardware_free_mailbox_can(&p_self->p_send_pcu_dv);
  hardware_free_mailbox_can(&p_self->p_mailbox_recv_asms);
  shared_message_read_destroy(&p_self->m_recv_mission_status);
  hardware_free_mailbox_can(&p_self->p_send_car_m_car_status_mailbox);
  as_node_mut_destroy(&p_self->m_as_node);
  EmergencyNode_destroy(&p_self->m_emergency_node);
  gpio_read_destroy(&p_self->m_gpio_air_precharge_done);
  gpio_read_destroy(&p_self->m_gpio_air_precharge_init);
  NS_ASSIS_SOUND(destroy)(&p_self->m_assi_sound);
  gpio_destroy(&p_self->m_gpio_ass_light_blue);
  gpio_destroy(&p_self->m_gpio_ass_light_yellow);
  lock_mission_ref_destroy_mut(&p_self->m_mission_locker);
  imu_destroy(&p_self->m_imu);
  ebs_destroy(&p_self->m_dv_ebs);
  NS_RES(destroy)(&p_self->m_dv_res);

  return 0;
}
