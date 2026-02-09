#include "cooling.h"
#include "../log/log.h"
#include "../../../../lib/raceup_board/raceup_board.h"
#include "../../../../cores/core_utility/core_utility.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include "../../../../lib/board_dbc/dbc/out_lib/can2/can2.h"
#pragma GCC diagnostic pop

#include <stdint.h>

#define TEMPS()\
X(temp_post_rr)\
X(temp_post_inv_l)\
X(temp_post_rl)\
X(temp_post_rad_r)\
X(temp_post_fr)\
X(temp_post_rad_l)\
X(temp_pump_left)\
X(temp_pump_right)\
X(temp_post_inv_r)\
X(temp_post_emb)\
X(temp_post_fl)\
X(current_fanbattl)\
X(current_fanbattr)\
X(current_fanradl)\
X(current_fanradr)\
X(current_pumpr)\
X(current_pumpl)\


enum TEMPS{
#define X(A) A,
  TEMPS()
#undef X
  __NUM_OF_TEMPS__
};

struct Cooling_t{
  uint32_t temps[__NUM_OF_TEMPS__];
  struct CanMailbox* p_recv_mailbox_cooling_temp_smu_r_1;
  struct CanMailbox* p_recv_mailbox_cooling_temp_smu_r_2;
  struct CanMailbox* p_recv_mailbox_cooling_temp_imu;
  struct CanMailbox* p_recv_mailbox_cooling_temp_smu_f_r;
  struct CanMailbox* p_recv_mailbox_fans;
  struct CanMailbox* p_recv_mailbox_current_pumps;
};

union Cooling_h_t_conv{
  Cooling_h* const hidden;
  struct Cooling_t* const clear;
};

#ifdef DEBUG
char __assert_size_cooling[(sizeof(Cooling_h)==sizeof(struct Cooling_t))?1:-1];
char __assert_align_cooling[(_Alignof(Cooling_h)==_Alignof(struct Cooling_t))?1:-1];
#endif /* ifdef DEBUG */

//public

int8_t cooling_init(Cooling_h* const restrict self)
{
  union Cooling_h_t_conv conv = {self};
  struct Cooling_t* p_self = conv.clear;
  struct CanNode* p_node = NULL;
  int32_t size = 0;

#define INIT_MAILBOX(MAILBOX,STRING,ID)\
  while (!p_self->MAILBOX)\
  {\
    serial_write_str(STRING);\
    ACTION_ON_CAN_NODE(CAN_GENERAL, p_node)\
    {\
      size = message_dlc_can2(ID);\
      if (size>0)\
      {\
        p_self->MAILBOX =\
          hardware_get_mailbox_single_mex(\
              p_node,\
              RECV_MAILBOX,\
              ID,\
              (uint8_t)size);\
      }\
    }\
  }

  INIT_MAILBOX(
      p_recv_mailbox_cooling_temp_smu_f_r,
      "init cooling mailbox smu front",
      CAN_ID_TEMPFRONTR)

  INIT_MAILBOX(
      p_recv_mailbox_cooling_temp_imu,
      "init cooling mailbox imu",
      CAN_ID_TEMPIMU)

  INIT_MAILBOX(
      p_recv_mailbox_cooling_temp_smu_r_1,
      "init cooling mailbox temp 1",
      CAN_ID_TEMP1)

  INIT_MAILBOX(
      p_recv_mailbox_cooling_temp_smu_r_2,
      "init cooling mailbox temp 2",
      CAN_ID_TEMP2)

  INIT_MAILBOX(p_recv_mailbox_current_pumps,
      "init current pumps mailbox",
      CAN_ID_PCUADC1);

  INIT_MAILBOX(p_recv_mailbox_fans,
      "init fans mailbox",
      CAN_ID_PCUADC2);


#define ADD_ENTRY(NAME,TEMP)\
  {\
    LogEntry_h entry =\
    {\
      .data_format = UNSIGNED,\
      .log_mode = LOG_SD | LOG_TELEMETRY,\
      .data_ptr = &p_self->temps[TEMP],\
      .data_mode = __u32__,\
      .name = NAME,\
    };\
    while(log_add_entry(&entry)<0);\
  }

#define X(A) ADD_ENTRY(#A,A)
  TEMPS()
#undef X


  return 0;
}

int8_t cooling_update(Cooling_h* const restrict self)
{
  union Cooling_h_t_conv conv = {self};
  struct Cooling_t* p_self = conv.clear;
  can_obj_can2_h_t o2 = {0};
  CanMessage mex = {0};

#define UPDATE_MAILBOX(M,ID,UPDATE_SCOPE)\
  if (hardware_mailbox_read(p_self->M, &mex)>0)\
  {\
    if(unpack_message_can2(&o2, ID, mex.full_word, (uint8_t) message_dlc_can2(ID), 0)<0)\
    {\
      SET_TRACE(CORE_1, UNREACHABLE_CODE);\
    }\
    else\
    UPDATE_SCOPE\
  }

  UPDATE_MAILBOX(p_recv_mailbox_cooling_temp_imu, CAN_ID_TEMPIMU,{
      p_self->temps[temp_pump_left] = o2.can_0x115_TempImu.temp_pump_left;
      p_self->temps[temp_pump_right] = o2.can_0x115_TempImu.temp_pump_right;
      })

  UPDATE_MAILBOX(p_recv_mailbox_cooling_temp_smu_f_r, CAN_ID_TEMPFRONTR,{
      p_self->temps[temp_post_inv_r] = o2.can_0x105_TempFrontR.temp_post_inv_r;
      p_self->temps[temp_post_emb] = o2.can_0x105_TempFrontR.temp_post_emb;
      })

  UPDATE_MAILBOX(p_recv_mailbox_cooling_temp_smu_r_1, CAN_ID_TEMP1,{
      p_self->temps[temp_post_rr] = o2.can_0x100_Temp1.temp_post_rr;
      p_self->temps[temp_post_rad_r] = o2.can_0x100_Temp1.temp_post_rad_r;
      p_self->temps[temp_post_rad_l] = o2.can_0x100_Temp1.temp_post_rad_l;
      p_self->temps[temp_post_fr] = o2.can_0x100_Temp1.temp_post_fr;
      })

  UPDATE_MAILBOX(p_recv_mailbox_cooling_temp_smu_r_2, CAN_ID_TEMP2,{
      p_self->temps[temp_post_inv_l] = o2.can_0x101_Temp2.temp_post_inv_l;
      p_self->temps[temp_post_rl] = o2.can_0x101_Temp2.temp_post_rl;
      p_self->temps[temp_post_fl] = o2.can_0x101_Temp2.temp_post_fl;
      })

  UPDATE_MAILBOX(p_recv_mailbox_current_pumps, CAN_ID_PCUADC1,{
      p_self->temps[current_pumpr] = o2.can_0x14b_PcuAdc1.adc_pumpr;
      p_self->temps[current_pumpl] = o2.can_0x14b_PcuAdc1.adc_pumpl;
      });

  UPDATE_MAILBOX(p_recv_mailbox_fans, CAN_ID_PCUADC2,{
      p_self->temps[current_fanbattl] = o2.can_0x14c_PcuAdc2.adc_fanbattl;
      p_self->temps[current_fanbattr] = o2.can_0x14c_PcuAdc2.adc_fanbattr;
      p_self->temps[current_fanradl] = o2.can_0x14c_PcuAdc2.adc_fanradl;
      p_self->temps[current_fanradr] = o2.can_0x14c_PcuAdc2.adc_fanradr;
      });

  return 0;
}
