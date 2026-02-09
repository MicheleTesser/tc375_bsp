#ifndef __CONTROLL_FREQUENCY__
#define __CONTROLL_FREQUENCY__

#include <stdint.h>

#define MAX_NUM_OF_CONTROL_FREQ 32

typedef struct __attribute__((aligned(4)))
{
  const uint8_t private_data[16];
}ControlFreq_h;

__attribute__((__nonnull__(1,2,3)))
int8_t control_freq_init(ControlFreq_h* const restrict self,
    const char* const name_duration, const char* name_delay);

__attribute__((__nonnull__(1)))
int8_t control_freq_tick_start(ControlFreq_h* const restrict self);

__attribute__((__nonnull__(1)))
int8_t control_freq_tick_end(ControlFreq_h* const restrict self);

#define MONITOR_FREQ_ACTION(p_freq)\
  control_freq_tick_start(&p_freq);\
  for (uint8_t node_used=0;!node_used;(control_freq_tick_end(&p_freq), node_used=1))

#endif // !__CONTROLL_FREQUENCY__
