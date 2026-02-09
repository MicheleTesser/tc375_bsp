#include "core_2.h"
#include "../core_utility/core_utility.h"
#include "feature/feature.h"
#include <stdint.h>

//public

void main_2(void)
{
  //setup
  CoreAliveBlink_h alive_blink={0};
  Core2Feature_h feature={0};
  ControlFreq_h freq_core = {0};

  while (core_alive_blink_init(&alive_blink, GPIO_CORE_2_ALIVE_BLINK, get_tick_from_millis(600)) <0)
  {
    serial_write_str("alive blink init core 2 failed");
  }

  while (core_2_feature_init(&feature)<0)
  {
    serial_write_str("core 2 init feature failed");
  }

  while (control_freq_init(&freq_core, "FREQ_core_2_duration","FREQ_core_2_delay"))
  {
    serial_write_str("init core 2 freq analysis failed");
    timer_wait(get_tick_from_millis(200));
  }

  // cores sync
  core_status_core_ready(CORE_2);
  while (!core_status_ready_state());

  //loop
  for(;;)
  {
    MONITOR_FREQ_ACTION(freq_core)
    {
      core_alive_blink_update(&alive_blink);
      core_2_feature_update(&feature);

      errno_trace_print(CORE_2);
      errno_trace_clear(CORE_2);
    }
  }
}
