#include "hv.h"
#include "../../../../../../lib/raceup_board/raceup_board.h"
#include "../../../../../core_utility/core_utility.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include "../../../../../../lib/board_dbc/dbc/out_lib/can2/can2.h"
#pragma GCC diagnostic pop 
#include <stdint.h>
#include <string.h>

struct GieiHv_t{
  struct CanMailbox* p_recv_lem;
  struct CanMailbox* p_recv_bms_hv_1;
  struct CanMailbox* p_recv_bms_hv_2;
  struct CanMailbox* send_mailbox_bms_hv;
  float hv_public_data[__NUM_OF_GIEI_HV_INFO__];
  float pack_tension;
};

union GieiHv_conv{
  Hv_h* const hidden;
  struct GieiHv_t* const clear;
};

union GieiHv_const_conv{
  const Hv_h* const  hidden;
  const struct GieiHv_t* const  clear;
};

#ifdef DEBUG
char __assert_size_GieiHv[(sizeof(Hv_h) == sizeof(struct GieiHv_t))? 1:-1];
char __assert_align_GieiHv[(_Alignof(Hv_h) == _Alignof(struct GieiHv_t))? 1:-1];
#endif // DEBUG

static int32_t _read_lem(const uint8_t raw_data[8]) {
  int32_t current = 0;
  uint32_t tmp = raw_data[0];
  tmp ^= 1<<7;
  current |= ((tmp) << 24u);
  current |= ((raw_data[1]) << 16);
  current |= ((raw_data[2]) << 8);
  current |= ((raw_data[3]) << 0);
  return current;
}


//public

int8_t hv_init(Hv_h* const restrict self)
{
  union GieiHv_conv conv = {self};
  struct GieiHv_t* p_self = conv.clear;
  struct CanNode* can_node = NULL;
  int8_t err=0;

  memset(p_self, 0, sizeof(*p_self));

  while (!p_self->p_recv_lem)
  {
    serial_write_str("init mailbox hv.c recv_lem");
    ACTION_ON_CAN_NODE(CAN_GENERAL, can_node)
    {
      p_self->p_recv_lem = hardware_get_mailbox_single_mex(
          can_node,
          RECV_MAILBOX,
          CAN_ID_LEM,
          (uint16_t)message_dlc_can2(CAN_ID_LEM));
    }
  }

  ACTION_ON_CAN_NODE(CAN_GENERAL,can_node)
  {
    p_self->send_mailbox_bms_hv =
      hardware_get_mailbox_single_mex(
          can_node,
          SEND_MAILBOX,
          CAN_ID_INVVOLT,
          (uint16_t)message_dlc_can2(CAN_ID_INVVOLT));

  }

  if (!p_self->send_mailbox_bms_hv)
  {
    goto hv_mailbox_failed;
  
  }

  ACTION_ON_CAN_NODE(CAN_GENERAL,can_node)
  {
    p_self->p_recv_bms_hv_1 =
      hardware_get_mailbox_single_mex(
          can_node,
          RECV_MAILBOX,
          CAN_ID_BMSHV1,
          (uint16_t)message_dlc_can2(CAN_ID_BMSHV1));

  }

  if (!p_self->p_recv_bms_hv_1)
  {
    goto bms_1_failed;
  
  }

  ACTION_ON_CAN_NODE(CAN_GENERAL,can_node)
  {
    p_self->p_recv_bms_hv_2 =
      hardware_get_mailbox_single_mex(
          can_node,
          RECV_MAILBOX,
          CAN_ID_BMSHV2,
          (uint16_t)message_dlc_can2(CAN_ID_BMSHV2));

  }

  if (!p_self->p_recv_bms_hv_2)
  {
    goto bms_2_failed;
  
  }

#define PUSH_VAR(NAME,PTR)\
  {\
    LogEntry_h entry = \
    {\
      .log_mode = LOG_TELEMETRY | LOG_SD,\
      .data_mode = __float__,\
      .name = NAME,\
      .data_ptr = &p_self->PTR,\
      .data_format = FLOAT,\
    };\
    while (log_add_entry(&entry))\
    {\
      serial_write_str("loggin var: " NAME " failed");\
    }\
  }

  PUSH_VAR("car_voltage", pack_tension);
  PUSH_VAR("total_power", hv_public_data[HV_TOTAL_POWER]);
  PUSH_VAR("lem_current", hv_public_data[HV_CURRENT_AMPERE]);

  PUSH_VAR("min_hv_volt", hv_public_data[HV_MIN_M_VOLT]);
  PUSH_VAR("max_hv_volt", hv_public_data[HV_MAX_M_VOLT]);
  PUSH_VAR("avg_hv_volt", hv_public_data[HV_AVG_M_VOLT]);

  PUSH_VAR("min_hv_temp", hv_public_data[HV_MIN_TEMP]);
  PUSH_VAR("max_hv_temp", hv_public_data[HV_MAX_TEMP]);
  PUSH_VAR("avg_hv_temp", hv_public_data[HV_AVG_TEMP]);

  PUSH_VAR("hv_soc", hv_public_data[HV_SOC]);


  return 0;

  err--;
  hardware_free_mailbox_can(&p_self->p_recv_bms_hv_2);
bms_2_failed:
  err--;
  hardware_free_mailbox_can(&p_self->p_recv_bms_hv_1);
bms_1_failed:
  err--;
  hardware_free_mailbox_can(&p_self->send_mailbox_bms_hv);
hv_mailbox_failed:
  err--;
  hardware_free_mailbox_can(&p_self->p_recv_lem);


  return err;
}

int8_t hv_update(Hv_h* const restrict self)
{
  union GieiHv_conv conv = {self};
  struct GieiHv_t* p_self = conv.clear;
  can_obj_can2_h_t o2= {0};
  CanMessage mex = {0};

  if(hardware_mailbox_read(p_self->p_recv_lem,&mex)>0)
  {
    p_self->hv_public_data[HV_CURRENT_AMPERE]=((float) _read_lem((uint8_t*)&mex.full_word))/1000;
  }

  if (hardware_mailbox_read(p_self->p_recv_bms_hv_1, &mex))
  {
    if(unpack_message_can2(&o2, 
          CAN_ID_BMSHV1,
          mex.full_word,
          (uint8_t)message_dlc_can2(CAN_ID_BMSHV1), 0)<0)
    {
      SET_TRACE(CORE_0, UNPACK_ERROR);
    }
    else
    {
      p_self->hv_public_data[HV_MIN_M_VOLT] = o2.can_0x057_BmsHv1.min_volt/10.0f;
      p_self->hv_public_data[HV_MAX_M_VOLT] = o2.can_0x057_BmsHv1.max_volt/10.0f;
      p_self->hv_public_data[HV_AVG_M_VOLT] = o2.can_0x057_BmsHv1.avg_volt/10.0f;
      p_self->hv_public_data[HV_SOC] = o2.can_0x057_BmsHv1.soc;
    }
  
  }

  if (hardware_mailbox_read(p_self->p_recv_bms_hv_2, &mex))
  {
    if(unpack_message_can2(&o2, 
          CAN_ID_BMSHV2,
          mex.full_word,
          (uint8_t)message_dlc_can2(CAN_ID_BMSHV2), 0)<0)
    {
      SET_TRACE(CORE_0, UNPACK_ERROR);
    }
    else
    {
      p_self->hv_public_data[HV_MIN_TEMP] = o2.can_0x058_BmsHv2.min_temp;
      p_self->hv_public_data[HV_MAX_TEMP] = o2.can_0x058_BmsHv2.max_temp;
      p_self->hv_public_data[HV_AVG_TEMP] = o2.can_0x058_BmsHv2.avg_temp;
      p_self->hv_public_data[HV_FAN_SPEED] = o2.can_0x058_BmsHv2.fan_speed;
    }
  }

  return 0;
}

float hv_get_info(const Hv_h* const restrict self, const enum GIEI_HV_INFO info)
{
  union GieiHv_const_conv conv = {self};
  const struct GieiHv_t* const p_self = conv.clear;

  if (info!=__NUM_OF_GIEI_HV_INFO__)
  {
    return p_self->hv_public_data[info];
  }
  SET_TRACE(CORE_0, UNREACHABLE_CODE);
  return -1;
}

/*
 * Battery pack tension is given indipendently by every motor.
 * The function seems complex because takes in consideration the case
 * that one or more motor are inactive.
 *
 * BMS precharge needs a message with the tot voltage
 */
int8_t hv_computeBatteryPackTension(Hv_h* const restrict self, 
    const float* const engines_voltages, const uint8_t num_of_voltages)
{
  union GieiHv_conv conv = {self};
  struct GieiHv_t* const restrict p_self = conv.clear;
  uint8_t active_motors = 0;
  float sum = 0.0f;
  float max = 0;
  can_obj_can2_h_t o;
  uint64_t data_mex=0;

  // find max voltage
  for (uint8_t i = 0; i < num_of_voltages; i++)
  {
    if (engines_voltages[i] > max)
    {
      max = engines_voltages[i];
    }
  }

  // Compute sum of voltages, exclude if it is below 50 V than the maximum reading
  for (uint8_t i = 0; i < num_of_voltages; i++)
  {
    if (engines_voltages[i] > (max - 50))
    {
      active_motors++;
      sum += engines_voltages[i];
    }
  }

  if (!active_motors) {
    p_self->hv_public_data[HV_BATTERY_PACK_TENSION]= 0;
    p_self->hv_public_data[HV_TOTAL_POWER]= 0;
  }
  else {
    p_self->pack_tension = sum/ active_motors;
    p_self->hv_public_data[HV_BATTERY_PACK_TENSION]= p_self->pack_tension;
    p_self->hv_public_data[HV_TOTAL_POWER]= p_self->pack_tension * p_self->hv_public_data[HV_CURRENT_AMPERE];
  }

  o.can_0x120_InvVolt.car_voltage = (uint16_t) p_self->pack_tension;
  pack_message_can2(&o, CAN_ID_INVVOLT, &data_mex);
  return hardware_mailbox_send(p_self->send_mailbox_bms_hv, data_mex);
}

int8_t hv_destroy(Hv_h* const restrict self)
{
  union GieiHv_conv conv = {self};
  struct GieiHv_t* const restrict p_self = conv.clear;

  hardware_free_mailbox_can(&p_self->p_recv_lem);
  hardware_free_mailbox_can(&p_self->send_mailbox_bms_hv);

  memset(self, 0, sizeof(*self));
  return 0;
}
