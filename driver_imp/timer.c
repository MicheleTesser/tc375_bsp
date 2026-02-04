#include "component.h"

#include "IfxPort.h"
#include "Bsp.h"

#include <stdint.h>


void timer_init(void)
{
}


time_var_microseconds timer_time_now(void)
{
  return now();
}

void timer_wait(const time_var_microseconds ticks)
{
  waitTime(ticks);
}

time_var_microseconds get_tick_from_millis(uint32_t millis)
{
  return IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, millis);
}

time_var_microseconds get_tick_from_micros(uint32_t micros)
{
  return IfxStm_getTicksFromMicroseconds(BSP_DEFAULT_TIMER, micros);
}

float get_millis_from_tick(time_var_microseconds ticks)
{
    sint32 freq = (sint32)IfxStm_getFrequency(IFXSTM_DEFAULT_TIMER);

    return (ticks * (1000))/freq;
}

float get_seconds_from_tick(time_var_microseconds ticks)
{
    sint32 freq = (sint32)IfxStm_getFrequency(IFXSTM_DEFAULT_TIMER);

    return ticks/freq;
}
