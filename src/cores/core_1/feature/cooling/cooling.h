#ifndef __COOLING__
#define __COOLING__

#include <stdint.h>
typedef struct __attribute__((aligned(4))){
  const uint8_t private_data[92];
}Cooling_h;

__attribute__((__nonnull__(1)))
int8_t cooling_init(Cooling_h* const restrict self);

__attribute__((__nonnull__(1)))
int8_t cooling_update(Cooling_h* const restrict self);

#endif // !__COOLING__
