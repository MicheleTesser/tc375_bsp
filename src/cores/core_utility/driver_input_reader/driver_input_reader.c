#include "driver_input_reader.h"
#include "../../../lib/raceup_board/raceup_board.h"
#include "../shared_message/shared_message.h"
#include "../emergency_module/emergency_module.h"
#include "../errno_trace/errno_trace.h"
#include "../log/log.h"
#include <stdint.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include "../../../lib/board_dbc/dbc/out_lib/can2/can2.h"
#include "../../../lib/board_dbc/dbc/out_lib/can3/can3.h"
#pragma GCC diagnostic pop 

#include <string.h>

struct DriverInputReader_t{
  struct{
    float m_throttle;
  }m_input[__NUM_OF_DRIVERS__ -1];
  float m_brake;
  uint8_t m_pot_impl;
  float m_steering_angle;
  SharedMessageReader_h m_recv_human_brake;
  SharedMessageReader_h m_recv_dv_driver;
  time_var_microseconds m_last_atc_received;
  EmergencyNode_h m_emergency_node;
};

union DriverInput_h_t_conv{
  DriverInputReader_h* const hidden;
  struct DriverInputReader_t* const clear;
};

union DriverInput_h_t_conv_const{
  const DriverInputReader_h* const hidden;
  const struct DriverInputReader_t* const clear;
};

enum {
  _ATC_NOT_ALIVE,
};

#ifdef DEBUG
char __assert_size_dv_driver_input_reader[(sizeof(DriverInputReader_h) == sizeof(struct DriverInputReader_t))? 1:-1];
char __assert_align_dv_driver_input_reader[(_Alignof(DriverInputReader_h) == _Alignof(struct DriverInputReader_t))? 1:-1];
#endif /* ifdef DEBUG */

static uint8_t LOG_DONE;

static inline float _saturate_100_f(float value)
{
  return value > 100.0f? 100.0f: value;
}

static inline float _saturate_90deg(float value)
{
  if (value > 90.0f)
  {
    return 90.0f;
  }
  else if (value < -90.0f)
  {
    return -90.0f;
  }

  return value;
}

//public

int8_t driver_input_reader_init(DriverInputReader_h* const restrict self)
{
  union DriverInput_h_t_conv conv ={self};
  struct DriverInputReader_t* const p_self = conv.clear;
  int8_t err=0;

  memset(p_self, 0, sizeof(*p_self));

  if(EmergencyNode_init(&p_self->m_emergency_node) < 0){
    goto emergency_node_failed;
  }

  if (shared_message_reader_init(&p_self->m_recv_human_brake, SHARED_MEX_DRIVER))
  {
    goto shared_mex_driver_failed;
  }

  if (shared_message_reader_init(&p_self->m_recv_dv_driver,SHARED_MEX_DV_DRIVER))
  {
    goto shared_mex_dv_driver_failed;
  }

#define ADD_ENTRY_TO_LOG(DATA, NAME)\
{\
    LogEntry_h entry= {\
        .data_mode = __float__,\
        .data_ptr = DATA,\
        .log_mode = LOG_SD | LOG_TELEMETRY,\
        .name = NAME,\
        .data_format = FLOAT,\
    };\
    while(log_add_entry(&entry)<0)\
    {\
      timer_wait(get_tick_from_millis(300));\
      serial_write_raw("driver input log init failed: ");\
      serial_write_str(NAME);\
    }\
}

  if (!LOG_DONE)
  {
    LOG_DONE = 1;
    ADD_ENTRY_TO_LOG(&p_self->m_input[DRIVER_HUMAN].m_throttle, "throttle_human");
    ADD_ENTRY_TO_LOG(&p_self->m_input[DRIVER_EMBEDDED].m_throttle, "throttle_dv");

    ADD_ENTRY_TO_LOG(&p_self->m_brake, "brake");
    ADD_ENTRY_TO_LOG(&p_self->m_pot_impl, "driver_pot_impl");

    ADD_ENTRY_TO_LOG(&p_self->m_steering_angle, "steering_angle");
  }


  return 0;

  err--;
emergency_node_failed:
  err--;
  shared_message_read_destroy(&p_self->m_recv_dv_driver);
shared_mex_dv_driver_failed:
  err--;
  shared_message_read_destroy(&p_self->m_recv_human_brake);
shared_mex_driver_failed:
  err--;

  return err;
}

int8_t driver_input_reader_update(DriverInputReader_h* const restrict self )
{
  typedef struct{
    uint8_t recv:1;
    uint8_t percentage;
  }BrakeReceived;

  union DriverInput_h_t_conv conv = {self};
  struct DriverInputReader_t* const restrict p_self = conv.clear;
  BrakeReceived brk_info[__NUM_OF_DRIVERS__] = {0};
  can_obj_can2_h_t o2={0};
  can_obj_can3_h_t o3={0};
  int8_t ret=0;

  if (shared_message_read_unpack_can2(&p_self->m_recv_human_brake, &o2)>0)
  {
    p_self->m_last_atc_received = timer_time_now();
    p_self->m_input[DRIVER_HUMAN].m_throttle = o2.can_0x053_Driver.throttle;
    p_self->m_steering_angle = o2.can_0x053_Driver.steering;
    p_self->m_pot_impl = o2.can_0x053_Driver.pot_implausibility;
    brk_info[DRIVER_HUMAN].recv=1;
    brk_info[DRIVER_HUMAN].percentage=o2.can_0x053_Driver.brake;
    ret=1;
  }

  if(timer_time_now() - p_self->m_last_atc_received > get_tick_from_millis(500)){
    SET_TRACE(CORE_1, MODULE_NOT_RESPONDING);
    EmergencyNode_raise(&p_self->m_emergency_node, _ATC_NOT_ALIVE);
  } else {
    EmergencyNode_solve(&p_self->m_emergency_node, _ATC_NOT_ALIVE);
  }

  if (shared_message_read_unpack_can3(&p_self->m_recv_dv_driver, &o3)>0)
  {
    const int8_t dv_brake_thr = o3.can_0x07d_DV_Driver.Throttle;

    if (dv_brake_thr>0)
    {
      p_self->m_input[DRIVER_EMBEDDED].m_throttle = o3.can_0x07d_DV_Driver.Throttle;
    }
    else
    {
      brk_info[DRIVER_EMBEDDED].recv=1;
      brk_info[DRIVER_EMBEDDED].percentage=dv_brake_thr>0?0:(uint8_t)-dv_brake_thr;
      p_self->m_input[DRIVER_EMBEDDED].m_throttle = 0;
    }
    ret=2;
  }

  if (brk_info[DRIVER_HUMAN].recv && brk_info[DRIVER_EMBEDDED].recv)
  {
    const uint8_t minor_brake = brk_info[DRIVER_HUMAN].percentage >= brk_info[DRIVER_EMBEDDED].percentage;
    p_self->m_brake =
      minor_brake * brk_info[DRIVER_HUMAN].percentage +
      (!minor_brake) * brk_info[DRIVER_EMBEDDED].percentage;
  }
  else if (brk_info[DRIVER_HUMAN].recv)
  {
    p_self->m_brake = brk_info[DRIVER_HUMAN].percentage;
  }
  else if (brk_info[DRIVER_EMBEDDED].recv)
  {
    p_self->m_brake = brk_info[DRIVER_EMBEDDED].percentage;
  }

  return ret;
}

float driver_input_reader_get(const DriverInputReader_h* const restrict self,
    const enum DRIVER driver_type,
    const enum INPUT_TYPES driver_input)
{
  const union DriverInput_h_t_conv_const conv = {self};
  const struct DriverInputReader_t* const p_self = conv.clear;

  if (driver_type == DRIVER_NONE || driver_type >= __NUM_OF_DRIVERS__)
  {
    return -1;
  }

  switch (driver_input)
  {
    case THROTTLE:
      return _saturate_100_f(p_self->m_input[driver_type].m_throttle);
    case BRAKE:
      return _saturate_100_f(p_self->m_brake);
    case STEERING_ANGLE:
      return _saturate_90deg(p_self->m_steering_angle);
    case __NUM_OF_INPUT_TYPES__:
    default:
      return -1;
  }

  return 0;
}

int8_t driver_input_reader_destroy(DriverInputReader_h* const restrict self)
{
  union DriverInput_h_t_conv conv ={self};
  struct DriverInputReader_t* const p_self = conv.clear;

  shared_message_read_destroy(&p_self->m_recv_dv_driver);
  shared_message_read_destroy(&p_self->m_recv_human_brake);

  return 0;
}
