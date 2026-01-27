#include "timer.h"
#include "FreeRTOSConfig.h"
#include "bsp.h"

void timer_init(void) {}

time_var_microseconds timer_time_now(void) {
  uint32_t raw = bsp_uc_stm_GetChannelCurrentValue(0U);
  return (time_var_microseconds)raw;
}

void timer_wait(const time_var_microseconds ticks) {
  bsp_uc_stm_Wait(configSTM_CLOCK_HZ, (uint32_t)ticks);
}

time_var_microseconds get_tick_from_millis(uint32_t millis) {
  return get_tick_from_micros(millis * 1000U);
}

time_var_microseconds get_tick_from_micros(uint32_t micros) {
  return (time_var_microseconds)((micros * (uint64_t)configSTM_CLOCK_HZ) /
                                 1000000ULL);
}

float get_millis_from_tick(time_var_microseconds ticks) {
  return (float)((ticks * 1000.0f) / (float)configSTM_CLOCK_HZ);
}

float get_seconds_from_tick(time_var_microseconds ticks) {
  return (float)(ticks / (float)configSTM_CLOCK_HZ);
}
