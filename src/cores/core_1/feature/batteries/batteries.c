#include "batteries.h"
#include "../../../../lib/raceup_board/raceup_board.h"
#include "../../../core_utility/core_utility.h"
#include "../log/log.h"
#include "serial.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include "../../../../lib/board_dbc/dbc/out_lib/can2/can2.h"
#pragma GCC diagnostic pop 

#include <stdint.h>
#include <string.h>

enum BMS_S{
  BMS_LV=0,

  __NUM_OF_BMS__
};

enum VOLTS{
  VOLTS_MAX=0,
  VOLTS_MIN,
  VOLTS_AVG,
  VOLTS_TOT,

  __NUM_OF_VOLTS__
};

enum TEMPS{
  TEMPS_MAX=0,
  TEMPS_MIN,

  __NUM_OF_TEMPS__
};

enum
{
  _EMERGENCY_NO_RECV_MESSAGES_LV,
  _EMERGENCY_LV_BATTERY_LEVEL,
};

struct CarBatteries_t{
  time_var_microseconds m_last_time_recv;
  uint16_t m_volts[__NUM_OF_VOLTS__];
  uint16_t m_temps[__NUM_OF_TEMPS__];
  uint32_t m_bms_lv_current;
  EmergencyNode_h m_emergency_node;
  struct CanMailbox* p_mailbox_volts;
  struct CanMailbox* p_recv_mailbox_temps;
  struct CanMailbox* p_recv_mailbox_bms_lv_emergency;
};

union CarBatteries_h_t_conv{
  CarBatteries_h* const restrict hidden;
  struct CarBatteries_t* const restrict clear;
};

#ifdef DEBUG
char __assert_size_core_1_batteries[(sizeof(CarBatteries_h) == sizeof(struct CarBatteries_t))? 1:-1];
char __assert_align_core_1_batteries[(_Alignof(CarBatteries_h) == _Alignof(struct CarBatteries_t))? 1:-1];
#endif // DEBUG

#define PUSH_ENTRY(VAR, NAME)\
  {\
    LogEntry_h entry = {\
      .data_mode = __u16__,\
      .data_format = UNSIGNED,\
      .data_ptr = VAR,\
      .log_mode = LOG_TELEMETRY| LOG_SD,\
      .name = NAME,\
    };\
    while(log_add_entry(&entry)<0)\
    {\
      serial_write_raw("failed init log var: ");\
      serial_write_raw(NAME);\
      timer_wait(get_tick_from_millis(200));\
    }\
  }

//public

int8_t car_batteries_init(CarBatteries_h* const restrict self)
{
  union CarBatteries_h_t_conv conv = {self};
  struct CarBatteries_t* const restrict p_self = conv.clear;
  int8_t err=0;
  struct CanNode* p_node = NULL;

  memset(p_self, 0, sizeof(*p_self));

  while ((err=EmergencyNode_init(&p_self->m_emergency_node))<0)
  {
    serial_write_raw("EmergencyNode_h init batteries: ");
    serial_write_int8_t(err);
    serial_write_str("");
  }

  while (!p_self->p_recv_mailbox_bms_lv_emergency)
  {
    ACTION_ON_CAN_NODE(CAN_GENERAL, p_node)
    {
      p_self->p_recv_mailbox_bms_lv_emergency =
        hardware_get_mailbox_single_mex(
            p_node,
            RECV_MAILBOX,
            CAN_ID_LVERROR,
            (uint16_t) message_dlc_can2(CAN_ID_LVERROR));
    }
  }

  while (!p_self->p_mailbox_volts)
  {
    ACTION_ON_CAN_NODE(CAN_GENERAL, p_node)
    {
      p_self->p_mailbox_volts=
        hardware_get_mailbox_single_mex(
            p_node,
            RECV_MAILBOX,
            CAN_ID_BMSLV1,
            (uint16_t) message_dlc_can2(CAN_ID_BMSLV1));
    }
  }

  while (!p_self->p_recv_mailbox_temps)
  {
    ACTION_ON_CAN_NODE(CAN_GENERAL, p_node)
    {
      p_self->p_recv_mailbox_temps=
        hardware_get_mailbox_single_mex(
            p_node,
            RECV_MAILBOX,
            CAN_ID_BMSLV2,
            (uint16_t) message_dlc_can2(CAN_ID_BMSLV2));
    }
  }


  PUSH_ENTRY(&p_self->m_bms_lv_current, "lv_lem_current");

  PUSH_ENTRY(&p_self->m_volts[VOLTS_MIN], "min_lv_volt");
  PUSH_ENTRY(&p_self->m_volts[VOLTS_MAX], "max_lv_volt");
  PUSH_ENTRY(&p_self->m_volts[VOLTS_AVG], "avg_lv_volt");
  PUSH_ENTRY(&p_self->m_volts[VOLTS_TOT], "tot_lv_volt");

  PUSH_ENTRY(&p_self->m_temps[TEMPS_MIN], "min_lv_temp");
  PUSH_ENTRY(&p_self->m_temps[TEMPS_MAX], "max_lv_temp");

  return 0;
}

int8_t car_batteries_update(CarBatteries_h* const restrict self)
{
  union CarBatteries_h_t_conv conv = {self};
  struct CarBatteries_t* const restrict p_self = conv.clear;
  can_obj_can2_h_t o = {0};
  CanMessage mex = {0};
  if (hardware_mailbox_read(p_self->p_mailbox_volts, &mex))
  {
    if(unpack_message_can2(&o, CAN_ID_BMSLV1, mex.full_word, 8, 0)<0)
    {
      SET_TRACE(CORE_0, UNPACK_ERROR);
    }
    else
    {
      p_self->m_volts[VOLTS_MAX] = o.can_0x054_BmsLv1.max_volt/10;
      p_self->m_volts[VOLTS_MIN] = o.can_0x054_BmsLv1.min_volt/10;
      p_self->m_volts[VOLTS_AVG] = o.can_0x054_BmsLv1.avg_volt/10;
      p_self->m_volts[VOLTS_TOT] = o.can_0x054_BmsLv1.tot_volt * 10;
      p_self->m_last_time_recv = timer_time_now();
      EmergencyNode_solve(&p_self->m_emergency_node, _EMERGENCY_NO_RECV_MESSAGES_LV);
    }
  }
  if (hardware_mailbox_read(p_self->p_recv_mailbox_temps, &mex))
  {
    if(unpack_message_can2(&o, CAN_ID_BMSLV2, mex.full_word, 8, 0)<0)
    {
      SET_TRACE(CORE_0, UNPACK_ERROR);
    }
    else
    {
      unpack_message_can2(&o, CAN_ID_BMSLV2, mex.full_word, (uint8_t) message_dlc_can2(CAN_ID_BMSLV2), 0);
      p_self->m_temps[TEMPS_MAX] = o.can_0x055_BmsLv2.max_temp;
      p_self->m_temps[TEMPS_MIN] = o.can_0x055_BmsLv2.min_temp;
      p_self->m_bms_lv_current = o.can_0x055_BmsLv2.current;
      EmergencyNode_solve(&p_self->m_emergency_node, _EMERGENCY_NO_RECV_MESSAGES_LV);
    }
  }
  // if ((timer_time_now() - p_self->m_last_time_recv) > get_tick_from_millis(1000)) HACK:
  // {
  //   SET_TRACE(CORE_1, MODULE_NOT_RESPONDING);
  // }

  return 0;
}

int8_t car_batteries_destroy(CarBatteries_h* const restrict self)
{
  union CarBatteries_h_t_conv conv = {self};
  struct CarBatteries_t* const restrict p_self = conv.clear;

  hardware_free_mailbox_can(&p_self->p_mailbox_volts);
  hardware_free_mailbox_can(&p_self->p_recv_mailbox_temps);
  hardware_free_mailbox_can(&p_self->p_recv_mailbox_bms_lv_emergency);

  memset(self, 0, sizeof(*self));
  return 0;
}
