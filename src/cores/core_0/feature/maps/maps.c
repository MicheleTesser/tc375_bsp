#include "maps.h"
#include "../../../core_utility/core_utility.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include "../../../../lib/board_dbc/dbc/out_lib/can2/can2.h"
#pragma GCC diagnostic pop
#include "../../../../lib/raceup_board/raceup_board.h"
#include <stdint.h>
#include <string.h>

enum MAPS_TYPE{
    MAPS_TYPE_POWER=0,
    MAPS_TYPE_REGEN,
    MAPS_TYPE_TV_REPARTITION,
};

struct PowerMap{
  struct{
    float power_kw;
    float torque_pos;
  }map_list[10];
  uint8_t active;
};

struct RegenMap{
  struct{
    float regen_scale;
    float max_neg_torque;
  }map_list[12];
  uint8_t active;
};

struct TvRepartitionMap{
  struct{
    float repartition;
    uint8_t tv_active:1;
    uint8_t tc_strat:3;
  }map_list[10];
  uint8_t active;
};

union BankToMaps{
  uint32_t data[2];
  struct 
  {
    uint8_t power;
    uint8_t regen;
    uint8_t tv_rep;
  };
};

struct DrivingMaps_t{
  struct PowerMap power_map;
  struct RegenMap regen_map;
  struct TvRepartitionMap tv_repartition_map;
  FlashMemoryBank_h active_maps;
  struct CanMailbox* map_mailbox;
  struct CanMailbox* map_ack_mailbox;
  struct CanMailbox* car_setting_mailbox;
  time_var_microseconds last_sent_ack;
};

union DrivingMaps_h_t_conv{
  DrivingMaps_h* const restrict hidden;
  struct DrivingMaps_t* const restrict clear;
};

union DrivingMaps_h_t_conv_const{
  const DrivingMaps_h* const restrict hidden;
  const struct DrivingMaps_t* const restrict clear;
};

#ifdef DEBUG
char __assert_size_core_0_maps[(sizeof(DrivingMaps_h)) == sizeof(struct DrivingMaps_t)? 1:-1];
char __assert_align_core_0_maps[(_Alignof(DrivingMaps_h)) == _Alignof(struct DrivingMaps_t)? 1:-1];
#endif // DEBUG

static inline void _set_pow_map(struct DrivingMaps_t* const restrict self,
    const uint8_t map_i, const float kw, const float torque)
{
  struct PowerMap* map = &self->power_map;
  map->map_list[map_i].power_kw=kw;
  map->map_list[map_i].torque_pos =torque;
}

static inline void _set_regen_map(struct DrivingMaps_t* const restrict self,
    const uint8_t map_i, const float regen_scale, const float torque)
{
  struct RegenMap* map = &self->regen_map;
  map->map_list[map_i].regen_scale = regen_scale;
  map->map_list[map_i].max_neg_torque = torque;
}

static inline void _set_repartition_map(struct DrivingMaps_t* const restrict self,
    const uint8_t map_i, const float repartition, const uint8_t tv, const uint8_t tc_strat)
{
  struct TvRepartitionMap* map = &self->tv_repartition_map;
  map->map_list[map_i].repartition = repartition;
  map->map_list[map_i].tv_active = tv;
  map->map_list[map_i].tc_strat = tc_strat;
}


static void _init_power_maps(struct DrivingMaps_t* const restrict self)
{
  _set_pow_map(self, 0, 35000, 13);
  _set_pow_map(self, 1, 50000, 15);
  _set_pow_map(self, 2, 70000, 18);
  _set_pow_map(self, 3, 78000, 21);
  _set_pow_map(self, 4, 70000, 21);
  _set_pow_map(self, 5, 50000, 18);
  _set_pow_map(self, 6, 75000, 20);
  _set_pow_map(self, 7, 75000, 21);
  _set_pow_map(self, 8, 10000, 5);
  _set_pow_map(self, 9, 10000, 10);
}

static void _init_regen_maps(struct DrivingMaps_t* const restrict self)
{
  _set_regen_map(self, 0, 0, 0);
  _set_regen_map(self, 1, 30, 13);
  _set_regen_map(self, 2, 50, 15);
  _set_regen_map(self, 3, 70, 15);
  _set_regen_map(self, 4, 70, 18);
  _set_regen_map(self, 5, 100, 15);
  _set_regen_map(self, 6, 100, 18);
  _set_regen_map(self, 7, 100, 21);
  _set_regen_map(self, 8, 150, 18);
  _set_regen_map(self, 9, 150, 21);
  _set_regen_map(self, 10, 70, 15);
  _set_regen_map(self, 11, 90, 15);
}

static void _init_repartition_maps(struct DrivingMaps_t* const restrict self)
{
  _set_repartition_map(self, 0, 0.50f, 0,0);
  _set_repartition_map(self, 1, 0.60f, 0,0);
  _set_repartition_map(self, 2, 0.70f, 0,0); 
  _set_repartition_map(self, 3, 0.80f, 0,0); 
  _set_repartition_map(self, 4, 1.0f, 0,0); 
  _set_repartition_map(self, 5, 0.70f, 0,2);
  _set_repartition_map(self, 6, 0.70f, 1,2);
  _set_repartition_map(self, 7, 0.70f, 1,0);
  _set_repartition_map(self, 8, 0.80f, 1,0);
  _set_repartition_map(self, 9, 1.0f, 1,0); 
}

//public

int8_t driving_maps_init(DrivingMaps_h* const restrict self)
{
  union DrivingMaps_h_t_conv conv = {self};
  struct DrivingMaps_t* const restrict p_self = conv.clear;
  struct CanNode* can_node = NULL;
  union BankToMaps data_bank={0};
  int8_t err=0;

  memset(p_self, 0, sizeof(*p_self));

  ACTION_ON_CAN_NODE(CAN_GENERAL,can_node)
  {
    p_self->map_mailbox =
      hardware_get_mailbox_single_mex(
          can_node,
          RECV_MAILBOX,
          CAN_ID_MAP,
          (uint16_t)message_dlc_can2(CAN_ID_MAP));
  }

  if (!p_self->map_mailbox)
  {
    goto mailbox_map_failed;
  }

  ACTION_ON_CAN_NODE(CAN_GENERAL,can_node)
  {
    p_self->map_ack_mailbox =
      hardware_get_mailbox_single_mex(
          can_node,
          SEND_MAILBOX,
          CAN_ID_MAPACK,
          (uint16_t)message_dlc_can2(CAN_ID_MAPACK));
  }

  if (!p_self->map_ack_mailbox)
  {
    goto mailbox_map_ack_failed;
  }

  ACTION_ON_CAN_NODE(CAN_GENERAL,can_node)
  {
    p_self->car_setting_mailbox =
      hardware_get_mailbox_single_mex(
          can_node,
          SEND_MAILBOX,
          CAN_ID_CARSETTINGS,
          (uint16_t)message_dlc_can2(CAN_ID_CARSETTINGS));
  }

  if (!p_self->car_setting_mailbox)
  {
    goto mailbox_car_settings_failed;
  }



  while(flash_memory_req_bank(&p_self->active_maps, 0)<0);


  _init_power_maps(p_self);
  _init_regen_maps(p_self);
  _init_repartition_maps(p_self);

  flash_memory_read_bank(&p_self->active_maps, data_bank.data);

  p_self->power_map.active = data_bank.power;
  p_self->regen_map.active = data_bank.regen;
  p_self->tv_repartition_map.active = data_bank.tv_rep;

  return 0;

  err--;
  hardware_free_mailbox_can(&p_self->car_setting_mailbox);
mailbox_car_settings_failed:
  err--;
  hardware_free_mailbox_can(&p_self->map_ack_mailbox);
mailbox_map_ack_failed:
  err--;
  hardware_free_mailbox_can(&p_self->map_mailbox);
mailbox_map_failed:
  err--;

  return err;
}

int8_t driving_map_update(DrivingMaps_h* const restrict self)
{
  union DrivingMaps_h_t_conv conv = {self};
  struct DrivingMaps_t* const restrict p_self = conv.clear;
  can_obj_can2_h_t o_ack, o_sett;
  CanMessage mex;
  if(hardware_mailbox_read(p_self->map_mailbox,&mex))
  {
    union BankToMaps data_bank={0};
    unpack_message_can2(&o_ack, CAN_ID_MAP, mex.full_word, 3, 0);
    p_self->power_map.active = o_ack.can_0x064_Map.power;
    p_self->regen_map.active = o_ack.can_0x064_Map.regen;
    p_self->tv_repartition_map.active = o_ack.can_0x064_Map.torque_rep;

    data_bank.power = p_self->power_map.active;
    data_bank.regen = p_self->regen_map.active;
    data_bank.tv_rep = p_self->tv_repartition_map.active;
    flash_memory_write_bank(&p_self->active_maps, data_bank.data);
  }

  ACTION_ON_FREQUENCY(p_self->last_sent_ack, get_tick_from_millis(3000))
  {
    o_ack.can_0x2bc_MapAck.power = p_self->power_map.active;
    o_ack.can_0x2bc_MapAck.regen = p_self->regen_map.active;
    o_ack.can_0x2bc_MapAck.torque_rep = p_self->tv_repartition_map.active;
    mex.full_word = 0;
    pack_message_can2(&o_ack, CAN_ID_MAPACK, &mex.full_word);
    hardware_mailbox_send(p_self->map_ack_mailbox, mex.full_word);
  
    o_sett.can_0x066_CarSettings.front_motor_repartition = (uint8_t) ((1 - driving_map_get_parameter(self, TORQUE_REPARTITION)) * 100);
    o_sett.can_0x066_CarSettings.rear_motor_repartition = (uint8_t) (driving_map_get_parameter(self, TORQUE_REPARTITION) * 100);
    o_sett.can_0x066_CarSettings.max_neg_trq = (int8_t) driving_map_get_parameter(self, MAX_NEG_TORQUE);
    o_sett.can_0x066_CarSettings.max_pos_trq = (uint8_t) driving_map_get_parameter(self, MAX_POS_TORQUE);
    o_sett.can_0x066_CarSettings.max_regen_current = (uint8_t) driving_map_get_parameter(self, REGEN_SCALE);
    o_sett.can_0x066_CarSettings.pwr_limit = (uint8_t) (driving_map_get_parameter(self, POWER_KW)/1000);
    o_sett.can_0x066_CarSettings.speed_lim = 0;
    o_sett.can_0x066_CarSettings.tc_strat = p_self->tv_repartition_map.map_list[p_self->tv_repartition_map.active].tc_strat;
    o_sett.can_0x066_CarSettings.torque_vectoring = p_self->tv_repartition_map.map_list[p_self->tv_repartition_map.active].tc_strat;
    mex.full_word = 0;
    pack_message_can2(&o_sett, CAN_ID_CARSETTINGS, &mex.full_word);
    hardware_mailbox_send(p_self->car_setting_mailbox, mex.full_word);
  }

  return 0;
}

float driving_map_get_parameter(const DrivingMaps_h* const restrict self,
    const enum CAR_PARAMETERS param)
{
  union DrivingMaps_h_t_conv_const conv = {self};
  const struct DrivingMaps_t* const restrict p_self = conv.clear;
  const struct PowerMap* power_map =NULL;
  const struct RegenMap* regen_map =NULL;
  const struct TvRepartitionMap* tv_repartition =NULL;

  switch (param)
  {
    case MAX_POS_TORQUE:
      power_map = &p_self->power_map;
      return power_map->map_list[power_map->active].torque_pos;
    case POWER_KW:
      power_map = &p_self->power_map;
      return power_map->map_list[power_map->active].power_kw;
    case MAX_NEG_TORQUE:
      regen_map = &p_self->regen_map;
      return regen_map->map_list[regen_map->active].max_neg_torque;
    case REGEN_SCALE:
      regen_map = &p_self->regen_map;
      return regen_map->map_list[regen_map->active].regen_scale;
    case TV_ON:
      tv_repartition = &p_self->tv_repartition_map;
      return tv_repartition->map_list[tv_repartition->active].tv_active;
    case TC_STRAT:
      tv_repartition = &p_self->tv_repartition_map;
      return tv_repartition->map_list[tv_repartition->active].tc_strat;
    case TORQUE_REPARTITION:
      tv_repartition = &p_self->tv_repartition_map;
      return tv_repartition->map_list[tv_repartition->active].repartition;
      break;
    }

  SET_TRACE(CORE_0,UNREACHABLE_CODE);
  return -1;
}

int8_t driving_maps_destroy(DrivingMaps_h* const restrict self )
{
  union DrivingMaps_h_t_conv conv = {self};
  struct DrivingMaps_t* const restrict p_self = conv.clear;

  hardware_free_mailbox_can(&p_self->map_mailbox);
  hardware_free_mailbox_can(&p_self->car_setting_mailbox);
  hardware_free_mailbox_can(&p_self->map_ack_mailbox);

  memset(self, 0, sizeof(*self));

  return 0;
}
