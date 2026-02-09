#include "core_1.h"
#include "../core_utility/core_utility.h"
#include "feature/feature.h"
#include <stdint.h>


void main_1(void)
{
  //setup
  CoreAliveBlink_h alive_blink={0};
  Core1Feature_h feature = {0};
  ControlFreq_h freq_core = {0};
  int8_t err=0;

  while (hardware_init_flash_memory()<0)
  {
    serial_write_str("flash memory init failed");
  }

  while (core_alive_blink_init(&alive_blink, GPIO_CORE_1_ALIVE_BLINK, get_tick_from_millis(400)) <0)
  {
    serial_write_str("init core alive_blink core 1 failed");
  }

  while ((err=core_1_feature_init(&feature)) <0)
  {
    serial_write_raw("core 1 feature init failed: ");
    serial_write_int8_t(err);
    serial_write_str("");
  }

  while (control_freq_init(&freq_core, "FREQ_core_1_duration","FREQ_core_1_delay"))
  {
    serial_write_str("init core 1 freq analysis failed");
    timer_wait(get_tick_from_millis(200));
  }

  while (errno_trace_class_init()<0)
  {
    serial_write_str("core 1 errno_trace_class_init");
  }

  //cores sync
  core_status_core_ready(CORE_1);
  while (!core_status_ready_state());

  //loop
  for(;;)
  {
    MONITOR_FREQ_ACTION(freq_core)
    {
      core_alive_blink_update(&alive_blink);
      core_1_feature_update(&feature);

      errno_trace_print(CORE_1);
      errno_trace_clear(CORE_1);
    }
  }
}
