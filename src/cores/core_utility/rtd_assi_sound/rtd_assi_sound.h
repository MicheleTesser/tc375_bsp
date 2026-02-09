#ifndef __RTD_ASSI_SOUND__
#define __RTD_ASSI_SOUND__

#include <stdint.h>
#include "../../../lib/raceup_board/raceup_board.h"

#define NS_ASSIS_SOUND(name) RTD_ASSI_SOUND_##name

typedef struct __attribute__((aligned(4))){
  const uint8_t private_data[24];
}RtdAssiSound_h;

typedef struct
{
  uint8_t duty_cycle_min;
  uint8_t duty_cycle_max;
  time_var_microseconds duty_cycle_step;
  time_var_microseconds duration;
}AssiMetadata;

int8_t
NS_ASSIS_SOUND(init)(RtdAssiSound_h* const restrict self, const AssiMetadata* metadata)
  __attribute__((__nonnull__(1)));

void
NS_ASSIS_SOUND(update)(void);

int8_t
NS_ASSIS_SOUND(start)(RtdAssiSound_h* const restrict self)
  __attribute__((__nonnull__(1)));

int8_t
NS_ASSIS_SOUND(stop)(RtdAssiSound_h* const restrict self)
  __attribute__((__nonnull__(1)));

int8_t NS_ASSIS_SOUND(destroy)(RtdAssiSound_h* const restrict self)
  __attribute__((__nonnull__(1)));


#endif // !__RTD_ASSI_SOUND__
