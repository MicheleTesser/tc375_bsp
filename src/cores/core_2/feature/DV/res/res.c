#include "res.h"
#include "../../../../../lib/raceup_board/raceup_board.h"
#include "../../../../core_utility/core_utility.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include "../../../../../lib/board_dbc/dbc/out_lib/can3/can3.h"
#include "../../../../../lib/board_dbc/dbc/out_lib/can2/can2.h"
#pragma GCC diagnostic pop 

#include <stdint.h>
#include <string.h>
#include <stdatomic.h>

#define NS_RE(name) NS_RES(EMERGENCY_##name)
enum NS_RE()
{
  NS_RE(UNREACHABLE_CODE=0),
  NS_RE(NOT_RESPONDING),

  NS_RE(__NUM_OF__)
};

struct DvRes_t{
  enum NS_RT() m_res_status;
  time_var_microseconds m_start_go_timeout;
  time_var_microseconds m_last_res_mex;
  time_var_microseconds m_start_wait_res;
  struct CanMailbox* p_mailbox_recv_res_on;
  struct CanMailbox* p_mailbox_recv_res_data;
  struct CanMailbox* p_mailbox_send_res_change_op;
  uint8_t m_started;
  uint16_t m_started_ready_to_go; //HACK: for compatibility size with gcc-tricore-elf
  struct
  {
    EmergencyNode_h m_emergency_node;
  };

};

union DvRes_h_t_conv{
  DvRes_h* const restrict hidden;
  struct DvRes_t* const restrict clear;
};

union DvRes_h_t_conv_const{
  const DvRes_h* const restrict hidden;
  const struct DvRes_t* const restrict clear;
};

#ifdef DEBUG
char __assert_size_res[(sizeof(DvRes_h) == sizeof(struct DvRes_t))? 1:-1];
char __assert_align_res[(_Alignof(DvRes_h) == _Alignof(struct DvRes_t))? 1:-1];
#endif // DEBUG

//public

int8_t NS_RES(init)(DvRes_h* const restrict self )
{
  union DvRes_h_t_conv conv = {self};
  struct DvRes_t* const restrict p_self = conv.clear;
  struct CanNode* p_node = NULL;
  int8_t err=0;

  memset(p_self, 0, sizeof(*p_self));


  if (EmergencyNode_init(&p_self->m_emergency_node)<0)
  {
    goto emergency_node_failed; 
  }

  ACTION_ON_CAN_NODE(CAN_DV, p_node)
  {
    if (message_dlc_can3(CAN_ID_DV_RES_ON)>=0)
    {
      p_self->p_mailbox_recv_res_on =
        hardware_get_mailbox_single_mex(
            p_node,
            RECV_MAILBOX,
            CAN_ID_DV_RES_ON,
            (uint16_t) message_dlc_can3(CAN_ID_DV_RES_ON));
    }
  }

  if (!p_self->p_mailbox_recv_res_on)
  {
    goto mailbox_res_on_error;
  }

  ACTION_ON_CAN_NODE(CAN_DV, p_node)
  {
    p_self->p_mailbox_recv_res_data =
      hardware_get_mailbox_single_mex(
          p_node,
          RECV_MAILBOX,
          CAN_ID_DV_RES_MESSAGE,
          (uint16_t) message_dlc_can3(CAN_ID_DV_RES_MESSAGE));
  }

  if (!p_self->p_mailbox_recv_res_data)
  {
    goto mailbox_res_message;
  }

  ACTION_ON_CAN_NODE(CAN_DV, p_node)
  {
    p_self->p_mailbox_send_res_change_op=
      hardware_get_mailbox_single_mex(
          p_node,
          SEND_MAILBOX,
          CAN_ID_DV_RES_OPERATIONALMODE,
          (uint16_t) message_dlc_can3(CAN_ID_DV_RES_OPERATIONALMODE));
  }

  if (!p_self->p_mailbox_send_res_change_op)
  {
    goto mailbox_res_op_mode_error;
  }


  p_self->m_started =0;
  p_self->m_start_go_timeout = 0;
  p_self->m_res_status = NS_RT(OFF);
  NS_SH(update)(NS_SD(GO_STATUS), p_self->m_res_status);

  return err;

  err--;
  hardware_free_mailbox_can(&p_self->p_mailbox_send_res_change_op);
mailbox_res_op_mode_error:
  err--;
  hardware_free_mailbox_can(&p_self->p_mailbox_recv_res_data);
mailbox_res_message:
  err--;
  hardware_free_mailbox_can(&p_self->p_mailbox_recv_res_on);
mailbox_res_on_error:
  err--;
  EmergencyNode_destroy(&p_self->m_emergency_node);
emergency_node_failed:
  err--;

  return err;
}

int8_t NS_RES(update)(DvRes_h* const restrict self )
{
  union DvRes_h_t_conv conv = {self};
  struct DvRes_t* const restrict p_self = conv.clear;
  int8_t err=0;
  can_obj_can3_h_t o3 = {0};
  CanMessage mex = {0};
  const uint8_t allow_drive = 
    p_self->m_start_go_timeout &&
    (timer_time_now() - p_self->m_start_go_timeout) >= get_tick_from_millis(5000);

  if (!p_self->m_started)
  {
    p_self->m_last_res_mex = timer_time_now();
  }

  if (hardware_mailbox_read(p_self->p_mailbox_recv_res_on, &mex))
  {
    p_self->m_start_wait_res= timer_time_now();
  }
  if(p_self->m_start_wait_res && (timer_time_now() - p_self->m_start_wait_res) > get_tick_from_millis(2000))
  {
    p_self->m_start_wait_res = 0;
    o3.can_0x000_DV_RES_OperationalMode.Address = 0x11;
    o3.can_0x000_DV_RES_OperationalMode.RequestedState = 0x1;
    if(pack_message_can3(&o3, CAN_ID_DV_RES_OPERATIONALMODE, &mex.full_word)<0)
    {
      SET_TRACE(CORE_2, PACK_ERROR);
    }
    else if(hardware_mailbox_send(p_self->p_mailbox_send_res_change_op, mex.full_word)<0)
    {
      SET_TRACE(CORE_2, CAN_SEND);
    }
  }


  if (hardware_mailbox_read(p_self->p_mailbox_recv_res_data, &mex)>0)
  {
    p_self->m_started =1;
    EmergencyNode_solve(&p_self->m_emergency_node, NS_RE(NOT_RESPONDING));
    p_self->m_last_res_mex = timer_time_now();
    if (unpack_message_can3(&o3, CAN_ID_DV_RES_MESSAGE, mex.full_word, 8, 0)<0)
    {
      SET_TRACE(CORE_2,UNPACK_ERROR);
      err--;
    }
    else {
      if (!o3.can_0x191_DV_RES_Message.EStop1 ||
          !o3.can_0x191_DV_RES_Message.EStop2 ||
          o3.can_0x191_DV_RES_Message.RadioQuality < 10)
      {
        p_self->m_res_status = NS_RT(ERROR);
      } 
      else if(o3.can_0x191_DV_RES_Message.SwitchStatus || o3.can_0x191_DV_RES_Message.ButtonStatus)
      {
        p_self->m_res_status = allow_drive * NS_RT(DRIVE); //+ !allow_drive * NS_RT(OFF);
      }
    }
  }

  if (NS_RES(started)(self) && !NS_RES(check_alive)(self))
  {
    EmergencyNode_raise(&p_self->m_emergency_node, NS_RE(NOT_RESPONDING));
    p_self->m_res_status = NS_RT(ERROR);
    SET_TRACE(CORE_2, MODULE_NOT_RESPONDING);
    err--;
  }

  NS_SH(update)(NS_SD(GO_STATUS), p_self->m_res_status);

  return err;
}

uint8_t NS_RES(check_alive)(const DvRes_h* const restrict self )
{
  const union DvRes_h_t_conv_const conv = {self};
  const struct DvRes_t* const p_self = conv.clear;
  return (timer_time_now() - p_self->m_last_res_mex) < get_tick_from_millis(100);
}

uint8_t NS_RES(started)(const DvRes_h* const restrict self )
{
  const union DvRes_h_t_conv_const conv = {self};
  const struct DvRes_t* const p_self = conv.clear;

  return p_self->m_started;
}

int8_t NS_RES(start_time_go)(DvRes_h* const restrict self )
{
  union DvRes_h_t_conv conv = {self};
  struct DvRes_t* const restrict p_self = conv.clear;

  if (!p_self->m_start_go_timeout)
  {
    p_self->m_start_go_timeout = 1;
    p_self->m_start_go_timeout = timer_time_now();
  }
  return 0;
}

int8_t NS_RES(reset_start_time_go)(DvRes_h* self )
{
  union DvRes_h_t_conv conv = {self};
  struct DvRes_t* const restrict p_self = conv.clear;

  p_self->m_start_go_timeout = 0;

  return 0;
}

int8_t
NS_RES(destroy)(DvRes_h* const restrict self )
{
  union DvRes_h_t_conv conv = {self};
  struct DvRes_t* const restrict p_self = conv.clear;

  EmergencyNode_destroy(&p_self->m_emergency_node);
  hardware_free_mailbox_can(&p_self->p_mailbox_recv_res_data);
  hardware_free_mailbox_can(&p_self->p_mailbox_recv_res_on);
  hardware_free_mailbox_can(&p_self->p_mailbox_send_res_change_op);

  memset(self, 0, sizeof(*self));

  return 0;
}
