#ifndef __DV_RES__
#define __DV_RES__

#include <stdint.h>

#define NS_RES(name) RES_##name

typedef struct __attribute__((aligned(4))) {
  const uint8_t private_data[56];
}DvRes_h;

#define NS_RT(name) NS_RES(STATUS_##name)
enum NS_RT()
{
  NS_RT(OFF)=0,
  NS_RT(DRIVE),
  NS_RT(ERROR),

  NS_RT(__NUM_OF__)
};

int8_t
NS_RES(init)(DvRes_h* const restrict self )__attribute__((__nonnull__(1)));

int8_t
NS_RES(update)(DvRes_h* const restrict self )__attribute__((__nonnull__(1)));

uint8_t
NS_RES(check_alive)(const DvRes_h* const restrict self )__attribute__((__nonnull__(1)));

uint8_t
NS_RES(started)(const DvRes_h* const restrict self )__attribute__((__nonnull__(1)));

int8_t
NS_RES(start_time_go)(DvRes_h* const restrict self )__attribute__((__nonnull__(1)));

int8_t
NS_RES(reset_start_time_go)(DvRes_h* const restrict self )__attribute__((__nonnull__(1)));

int8_t
NS_RES(destroy)(DvRes_h* const restrict self )__attribute__((__nonnull__(1)));

#endif // !__DV_RES__
