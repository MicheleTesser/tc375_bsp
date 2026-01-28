#include "timer.h"
#include "bsp.h"

static inline uint32_t stm_src_mhz(void) { return (uint32_t)UC_STM_CLOCK; }
static inline uint32_t stm_div(void) { return SCU_CCUCON0.B.STMDIV; }

void timer_init(void) {}

time_var_microseconds timer_time_now(void) {
  uint32_t coreId = (uint32_t)bsp_uc_core_GetCurrentCore();
  uint32_t ticks = bsp_uc_stm_GetChannelCurrentValue(coreId);
  return (time_var_microseconds)(((uint64_t)ticks * (uint64_t)stm_div()) /
                                 (uint64_t)stm_src_mhz());
}

void timer_wait(const time_var_microseconds time_us) {
  int coreId = bsp_uc_core_GetCurrentCore();
  uint32_t stm_val = bsp_uc_stm_GetChannelCurrentValue(coreId);
  while ((uint32_t)(bsp_uc_stm_GetChannelCurrentValue(coreId) - stm_val) <
         (time_us * stm_src_mhz() / stm_div())) {
  }
}

time_var_microseconds get_tick_from_millis(uint32_t millis) {
  return get_tick_from_micros(millis * 1000U);
}

time_var_microseconds get_tick_from_micros(uint32_t micros) {
  return (time_var_microseconds)(((uint64_t)micros * (uint64_t)stm_src_mhz()) /
                                 (uint64_t)stm_div());
}

float get_millis_from_tick(time_var_microseconds ticks) {
  return ((float)ticks * (float)stm_div()) / ((float)stm_src_mhz() * 1000.0f);
}

float get_seconds_from_tick(time_var_microseconds ticks) {
  return ((float)ticks * (float)stm_div()) /
         ((float)stm_src_mhz() * 1000000.0f);
}
