#include "system_settings.h"
#include "../core_utility.h"
#include "../../../lib/DPS/dps.h"
#include "../../../lib/raceup_board/raceup_board.h"
#include "IfxCpu.h"

#include <stdint.h>
#include <string.h>

struct SytemSettingOwner_t{
  SharedMessageReader_h p_mailbox_recv_dps_mex;
};

union SytemSettingOwner_h_t_conv{
  SytemSettingOwner_h* const restrict hidden;
  struct SytemSettingOwner_t* const restrict clear;
};

#ifdef DEBUG
char __assert_size_system_settings[sizeof(SytemSettingOwner_h)==sizeof(struct SytemSettingOwner_t)?+1:-1];
char __assert_align_system_settings[_Alignof(SytemSettingOwner_h)==_Alignof(struct SytemSettingOwner_t)?+1:-1];
#endif /* ifdef DEBUG */

static struct{
  struct CanMailbox* p_mailbox_send_dps_mex;
  DpsSlave_h m_dps_slave;
  uint8_t init_done;
  IfxCpu_mutexLock m_taken;
}SYSTEM_MONITOR = {0};

#define ACTION_ON_MUTEX()\
  while(!IfxCpu_acquireMutex(&SYSTEM_MONITOR.m_taken));\
  for (uint8_t node_used=0;!node_used;(IfxCpu_releaseMutex(&SYSTEM_MONITOR.m_taken), node_used=1))


static int8_t _dps_send_fun(const DpsCanMessage* const restrict mex)
{
  return hardware_mailbox_send(SYSTEM_MONITOR.p_mailbox_send_dps_mex, mex->full_word);
}

static void _dps_wait_fun(void)
{
  timer_wait(get_tick_from_millis(100));
}

//public

int8_t system_settings_init(SytemSettingOwner_h* const restrict self)
{
  union SytemSettingOwner_h_t_conv conv = {self};
  struct SytemSettingOwner_t* const restrict p_self = conv.clear;
  struct CanNode* can_node = NULL;
  int32_t mex_dlc = 0;
  int8_t err=0;

  if (shared_message_reader_init(&p_self->p_mailbox_recv_dps_mex,SHARED_MEX_DPS_MASTER)<0)
  {
    goto init_recv_mailbox_failed;
  }

  ACTION_ON_CAN_NODE(CAN_GENERAL, can_node)
  {
    mex_dlc = message_dlc(CAN_ID_DPSSLAVEMEX);
    if (mex_dlc>=0)
    {
      SYSTEM_MONITOR.p_mailbox_send_dps_mex =
        hardware_get_mailbox_single_mex(
            can_node,
            SEND_MAILBOX,
            CAN_ID_DPSSLAVEMEX,
            (uint16_t) mex_dlc);
    }
  }

  if (!SYSTEM_MONITOR.p_mailbox_send_dps_mex)
  {
    goto init_send_mailbox_failed;
  }

  const char board_name[BOARD_NAME_LENGTH] = "CULO";
  if(dps_slave_init(
        &SYSTEM_MONITOR.m_dps_slave,
        _dps_send_fun,
        _dps_wait_fun,
        board_name, 0,
        CAN_ID_DPSMASTERMEX,
        CAN_ID_DPSSLAVEMEX)<0)
  {
    goto init_dps_failed;
  }

  dps_slave_start(&SYSTEM_MONITOR.m_dps_slave);


  SYSTEM_MONITOR.init_done = 1;

  return 0;

  dps_slave_destroy(&SYSTEM_MONITOR.m_dps_slave);
init_dps_failed:
  err--;
  hardware_free_mailbox_can(&SYSTEM_MONITOR.p_mailbox_send_dps_mex);
init_send_mailbox_failed:
  err--;
  shared_message_read_destroy(&p_self->p_mailbox_recv_dps_mex);
init_recv_mailbox_failed:
  err--;

  return err;
}

int8_t system_settings_update(SytemSettingOwner_h* const restrict self)
{
  union SytemSettingOwner_h_t_conv conv = {self};
  struct SytemSettingOwner_t* const restrict p_self = conv.clear;
  uint64_t data=0;

  if (shared_message_read(&p_self->p_mailbox_recv_dps_mex, &data))
  {
    DpsCanMessage dps_mex = 
    {
      .dlc = (uint8_t) message_dlc(CAN_ID_DPSMASTERMEX),
      .full_word = data,
      .id = CAN_ID_DPSMASTERMEX,
    };
    (void)dps_slave_check_can_command_recv(&SYSTEM_MONITOR.m_dps_slave, &dps_mex);
  }

  return 0;
}

int8_t system_settings_destroy(SytemSettingOwner_h* const restrict self)
{
  union SytemSettingOwner_h_t_conv conv = {self};
  struct SytemSettingOwner_t* const restrict p_self = conv.clear;

  shared_message_read_destroy(&p_self->p_mailbox_recv_dps_mex);
  dps_slave_destroy(&SYSTEM_MONITOR.m_dps_slave);

  memset(self, 0, sizeof(*self));
  SYSTEM_MONITOR.init_done = 0;

  return 0;
}

int8_t system_settings_push_var(const enum DPS_PRIMITIVE_TYPES type,
    void* const p_data, post_update post_update_f, const char name[VAR_NAME_LENGTH])
{
  int8_t err=0;

  if (!SYSTEM_MONITOR.init_done)
  {
    return -99;
  }

  ACTION_ON_MUTEX()
  {
    err= dps_monitor_primitive_var(
        &SYSTEM_MONITOR.m_dps_slave,
        type,
        p_data,
        post_update_f,
        name);
  }

  return err;
}
