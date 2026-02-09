#include "control_frequency.h"

#include "../../../lib/raceup_board/raceup_board.h"
#include "../log/log.h"

#include <stdint.h>
#include <stdatomic.h>
#include <string.h>

typedef float Hertz;

struct ControlFreq_t
{
  Hertz m_duration;
  Hertz m_time_from_last_execution;
  time_var_microseconds m_start;
};


union ControlFreq_h_t_conv{
  ControlFreq_h* const hidden;
  struct ControlFreq_t* const clear;
};

union ControlFreq_h_t_conv_const{
  const ControlFreq_h* const hidden;
  const struct ControlFreq_t* const clear;
};


#if DEBUG
char __assert_size_control_freq[sizeof(ControlFreq_h)==sizeof(struct ControlFreq_t)?1:-1];
char __assert_align_control_freq[_Alignof(ControlFreq_h)==_Alignof(struct ControlFreq_t)?1:-1];
#endif /* if DEBUG */

static inline float _tick_to_heartz(const time_var_microseconds tick_diff)
{
  return 1.0f/get_seconds_from_tick(tick_diff);
}

//public

int8_t control_freq_init(ControlFreq_h* const restrict self,
    const char* const name_duration, const char* name_delay)
{
  union ControlFreq_h_t_conv conv = {self};
  struct ControlFreq_t* p_self = conv.clear;

  LogEntry_h entry = 
  {
    .data_format = FLOAT,
    .data_mode = __float__,
    .data_ptr = &p_self->m_duration,
    .log_mode = LOG_TELEMETRY | LOG_SD,
  };
  strncpy(entry.name, name_duration, sizeof(entry.name)-1);
  while (log_add_entry(&entry)<0)
  {
    serial_write_raw("trying adding control duration log: ");
    serial_write_raw(name_duration);
    timer_wait(get_tick_from_millis(200));
  }

  entry.data_ptr = &p_self->m_time_from_last_execution;
  strncpy(entry.name, name_delay, sizeof(entry.name)-1);

  while (log_add_entry(&entry)<0)
  {
    serial_write_raw("trying adding control start delay log: ");
    serial_write_raw(name_delay);
    timer_wait(get_tick_from_millis(200));
  }

  return 0;
}

int8_t control_freq_tick_start(ControlFreq_h* const restrict self)
{
  union ControlFreq_h_t_conv conv = {self};
  struct ControlFreq_t* p_self = conv.clear;
  const time_var_microseconds current_time = timer_time_now();

  p_self->m_time_from_last_execution = _tick_to_heartz(current_time - p_self->m_start);
  p_self->m_start = current_time;

  return 0;
}

int8_t control_freq_tick_end(ControlFreq_h* const restrict self)
{
  union ControlFreq_h_t_conv conv = {self};
  struct ControlFreq_t* p_self = conv.clear;

  p_self->m_duration = _tick_to_heartz(timer_time_now() - p_self->m_start);

  return 0;
}
