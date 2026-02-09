#include "errno_trace.h"
#include "../core_status/core_status.h"
#include "../system_settings/system_settings.h"
#include "../../../lib/raceup_board/raceup_board.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include "../../../lib/board_dbc/dbc/out_lib/can2/can2.h"
#pragma GCC diagnostic pop

#include <stdatomic.h>
#include <stddef.h>
#include <stdint.h>

typedef struct{
  Trace m_err_buffer[TRACE_DEPTH];
  _Atomic uint8_t m_lock;
  ErrroTypes first_error;
  uint8_t m_cursor;
}CoreTraces;

static struct
{
  uint8_t active_core;
  CoreTraces m_core_trace[__NUM_OF_CORES__];
  struct CanMailbox* p_send_mailbox_errno_err;
  time_var_microseconds last_sent;
}CORE_TRACES;

#define VARLID_CORE(core) if (core >= __NUM_OF_CORES__) return -1;

//public

int8_t errno_trace_class_init(void)
{
  struct CanNode* p_node = NULL;

  ACTION_ON_CAN_NODE(CAN_GENERAL, p_node)
  {
    CORE_TRACES.p_send_mailbox_errno_err =
      hardware_get_mailbox_single_mex(
          p_node,
          SEND_MAILBOX,
          CAN_ID_VCUERRTRACE,
          (uint16_t)message_dlc_can2(CAN_ID_VCUERRTRACE));
  }

  if (!CORE_TRACES.p_send_mailbox_errno_err)
  {
    return -1;
  }

  CORE_TRACES.active_core = 0;
  while(system_settings_push_var(
        DPS_TYPES_UINT8_T,
        &CORE_TRACES.active_core,
        NULL, 
        "CXTRCE")<0)
  {
    serial_write_str("pushing CXSTRE");
    timer_wait(get_tick_from_millis(200));
  }

  return 0;
}

int8_t 
errno_trace_push_trace(
    const enum CORES core,
    const ErrroTypes err_type,
    const int32_t err,
    const char* const place)
{
  CoreTraces* core_trace = NULL;
  Trace* trace = NULL;

  VARLID_CORE(core);
  core_trace = &CORE_TRACES.m_core_trace[core];

  if (core_trace->m_cursor==TRACE_DEPTH)
  {
    return -2; //queue full
  }

  trace = &core_trace->m_err_buffer[core_trace->m_cursor++];

  trace->m_err = err;
  trace->p_place = place;

  if (!atomic_exchange(&core_trace->m_lock, 1U))
  {
    core_trace->first_error = err_type;
  }

  return 0;
}

int8_t errno_trace_get(const enum CORES core, ErrnoTraces* const restrict o_trace)
{
  CoreTraces* core_trace = NULL;

  VARLID_CORE(core);
  core_trace = &CORE_TRACES.m_core_trace[core];

  o_trace->p_traces = core_trace->m_err_buffer;
  o_trace->m_err_num = core_trace->m_cursor;

  return 0;
}

int8_t errno_trace_print(const enum CORES core)
{
  ErrnoTraces core_trace = {0};
  can_obj_can2_h_t o2 = {0};
  const Trace* trace = NULL;
  uint8_t tabs =0;
  uint64_t payload =0;

  ACTION_ON_FREQUENCY(CORE_TRACES.last_sent, get_tick_from_millis(20))
  {
    o2.can_0x320_VcuErrTrace.core_0_err = (uint8_t) CORE_TRACES.m_core_trace[CORE_0].first_error;
    o2.can_0x320_VcuErrTrace.core_1_err = (uint8_t) CORE_TRACES.m_core_trace[CORE_1].first_error;
    o2.can_0x320_VcuErrTrace.core_2_err = (uint8_t) CORE_TRACES.m_core_trace[CORE_2].first_error;

    if (pack_message_can2(&o2, CAN_ID_VCUERRTRACE, &payload)<0)
    {
      serial_write_str("ERROR CAN SEND ERROR TRACE");
    }
    else
    {
      hardware_mailbox_send(CORE_TRACES.p_send_mailbox_errno_err, payload);
      for (uint8_t i=0; i<__NUM_OF_CORES__; i++)
      {
        CORE_TRACES.m_core_trace[i].first_error =0;
        atomic_store(&CORE_TRACES.m_core_trace[i].m_lock, 0U);
      }
    }
  }


  if (core!=CORE_TRACES.active_core)
  {
    return -1;
  }

  if(errno_trace_get(CORE_TRACES.active_core, &core_trace)<0)
  {
    return -2;
  }

  if (!core_trace.m_err_num)
  {
    return 0;
  }

  serial_write_raw("error traceback core : ");
  serial_write_uint32_t(CORE_TRACES.active_core);
  serial_write_str("");

  for (int8_t i = 0; i < core_trace.m_err_num; i++)
  {
    trace = &core_trace.p_traces[i];
    for (uint8_t j=0; j<tabs; j++)
    {
      serial_write_raw("\t");
    }
    serial_write_raw(trace->p_place);
    serial_write_raw(" :" );
    serial_write_int32_t(trace->m_err);
    serial_write_str("");
    tabs++;
  }

  return 0;
}

int8_t errno_trace_clear(const enum CORES core)
{
  VARLID_CORE(core);

  CORE_TRACES.m_core_trace[core].m_cursor=0;
  return 0;
}
