#ifndef __CAR_LOG__
#define __CAR_LOG__

#include <stdint.h>

#include "../../../../lib/raceup_board/raceup_board.h"
#include "../../../core_utility/core_utility.h"

typedef struct __attribute__((aligned(4))){
  const uint8_t private_data[104];
}Log_h;

int8_t
log_init(Log_h* const restrict self )__attribute__((__nonnull__(1)));

int8_t
log_update_and_send(Log_h* const restrict self )__attribute__((__nonnull__(1)));

int8_t
log_destroy(Log_h* const restrict self )__attribute__((__nonnull__(1)));


#endif // !__CAR_LOG__
