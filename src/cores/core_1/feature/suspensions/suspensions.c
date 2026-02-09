#include "suspensions.h"
#include "../../../../lib/raceup_board/raceup_board.h"
#include "../log/log.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include "../../../../lib/board_dbc/dbc/out_lib/can2/can2.h"
#pragma GCC diagnostic pop 
#include <stdint.h>
#include <string.h>

enum SUSPS_MAILBOX{
    M_FRONT=0,
    M_REAR,

    __NUM_OF_SUSPS_MAILBOX__
};

struct Suspensions_t{
    float m_susps_value[__NUM_OF_SUSPS__];
    struct CanMailbox* p_recv_mailbox[__NUM_OF_SUSPS_MAILBOX__]; 
};

union Suspensions_h_t_conv {
    Suspensions_h* const restrict hidden;
    struct Suspensions_t* const restrict clear;
};

#define UPDATE_LOG(DATA, NAME, ERROR)\
{\
    LogEntry_h entry ={\
        .data_ptr = &DATA,\
        .log_mode = LOG_SD | LOG_TELEMETRY,\
        .data_mode = __u16__,\
        .name = NAME,\
        .data_format = UNSIGNED,\
    };\
    if (log_add_entry(&entry)<0)\
    {\
        goto ERROR;\
    }\
}

#ifdef DEBUG
char __assert_size_alive_suspensions[(sizeof(Suspensions_h) == sizeof(struct Suspensions_t))? 1:-1];
char __assert_align_alive_suspensions[(_Alignof(Suspensions_h) == _Alignof(struct Suspensions_t))? 1:-1];
#endif // DEBUG

int8_t suspensions_init(Suspensions_h* const restrict self)
{
    union Suspensions_h_t_conv conv = {self};
    struct Suspensions_t* const restrict p_self = conv.clear;
    struct CanNode* can_node = NULL;
    int8_t err=0;

    memset(p_self, 0, sizeof(*p_self));

    UPDATE_LOG(p_self->m_susps_value[SUSP_FRONT_LEFT] , "suspensions_fl", update_failed);
    UPDATE_LOG(p_self->m_susps_value[SUSP_FRONT_RIGHT] , "suspensions_fr", update_failed);
    UPDATE_LOG(p_self->m_susps_value[SUSP_REAR_LEFT] , "suspensions_rl", update_failed);
    UPDATE_LOG(p_self->m_susps_value[SUSP_REAR_RIGHT] , "suspensions_rr", update_failed);

    ACTION_ON_CAN_NODE(CAN_GENERAL,can_node)
    {
      p_self->p_recv_mailbox[M_FRONT] =
        hardware_get_mailbox_single_mex(
            can_node,
            RECV_MAILBOX,
            CAN_ID_SUSPFRONT,
            (uint16_t) message_dlc_can2(CAN_ID_SUSPFRONT));
      if (!p_self->p_recv_mailbox[M_FRONT])
      {
        goto mailbox_susp_front;
      }

      p_self->p_recv_mailbox[M_REAR] =
        hardware_get_mailbox_single_mex(
            can_node,
            RECV_MAILBOX,
            CAN_ID_SUSPREAR,
            (uint16_t) message_dlc_can2(CAN_ID_SUSPREAR));

      if (!p_self->p_recv_mailbox[M_REAR])
      {
        goto mailbox_susp_rear;
      }
    }


    return 0;

    err--;
    hardware_free_mailbox_can(&p_self->p_recv_mailbox[M_REAR]);
mailbox_susp_rear:
    err--;
    hardware_free_mailbox_can(&p_self->p_recv_mailbox[M_FRONT]);
mailbox_susp_front:
    err--;
update_failed:
    err--;

    return err;
}

int8_t suspensions_update(Suspensions_h* const restrict self)
{
    union Suspensions_h_t_conv conv = {self};
    struct Suspensions_t* const restrict p_self = conv.clear;

    CanMessage mex;
    can_obj_can2_h_t o;

    if(hardware_mailbox_read(p_self->p_recv_mailbox[M_FRONT], &mex)>0){
        unpack_message_can2(&o, CAN_ID_SUSPFRONT, mex.full_word, 8, (dbcc_time_stamp_t) timer_time_now());
        p_self->m_susps_value[SUSP_FRONT_LEFT] = o.can_0x104_SuspFront.susp_fl;
        p_self->m_susps_value[SUSP_FRONT_RIGHT] = o.can_0x104_SuspFront.susp_fr;
    }

    if(hardware_mailbox_read(p_self->p_recv_mailbox[M_REAR],&mex)>0){
        unpack_message_can2(&o, CAN_ID_SUSPREAR, mex.full_word, 8, (dbcc_time_stamp_t) timer_time_now());
        p_self->m_susps_value[SUSP_REAR_LEFT] = o.can_0x102_SuspRear.susp_rl;
        p_self->m_susps_value[SUSP_REAR_RIGHT] = o.can_0x102_SuspRear.susp_rr;
    }

    return 0;
}

int8_t suspensions_destroy(Suspensions_h* const restrict self)
{
  union Suspensions_h_t_conv conv = {self};
  struct Suspensions_t* const restrict p_self = conv.clear;

  hardware_free_mailbox_can(&p_self->p_recv_mailbox[M_REAR]);
  hardware_free_mailbox_can(&p_self->p_recv_mailbox[M_FRONT]);

  memset(self, 0, sizeof(*self));

  return 0;
}
