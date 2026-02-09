#include "core_0.h"
#include "../core_utility/core_utility.h"
#include "feature/feature.h"
#include <stdint.h>

//public

void main_0(void)
{
    //setup
    CoreAliveBlink_h alive_blink={0};
    Core0Feature_h feature={0};
    ControlFreq_h freq_core = {0};

    while (serial_setup(115200)<0);

    while (EmergencyNode_class_init()<0)
    {
      serial_write_str("init EmergencyNode class failed");
    }

    while(hardware_init_can(CAN_INVERTER, _1_MBYTE_S_)<0)
    {
      serial_write_str("init can inverter failed");
    }

    while (hardware_init_can(CAN_GENERAL, _500_KBYTE_S_) <0)
    {
      serial_write_str("init can general failed");
    }

    while (hardware_init_can(CAN_DV, _500_KBYTE_S_)<0)
    {
      serial_write_str("init can node dv failed");
    }

    while (core_alive_blink_init(&alive_blink, GPIO_CORE_0_ALIVE_BLINK, get_tick_from_millis(200)) <0)
    {
      serial_write_str("trying init core alive_blink core 0");
    }


    while (core_0_feature_init(&feature)<0)
    {
      serial_write_str("init core 0 feature failed");
    }

    while (control_freq_init(&freq_core, "FREQ_core_0_duration","FREQ_core_0_delay"))
    {
      serial_write_str("init core 0 freq analysis failed");
      timer_wait(get_tick_from_millis(200));
    }

    //cores sync
    core_status_core_ready(CORE_0);
    while (!core_status_ready_state());

    //loop
    for(;;){
     MONITOR_FREQ_ACTION(freq_core)
      {
        core_alive_blink_update(&alive_blink);
        core_0_feature_update(&feature);
        core_0_feature_compute_power(&feature);
        errno_trace_print(CORE_0);
        errno_trace_clear(CORE_0);
     }
    }
}
