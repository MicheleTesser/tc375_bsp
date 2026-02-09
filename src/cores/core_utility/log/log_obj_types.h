#ifndef __LOG_OBJ_TYPES__
#define __LOG_OBJ_TYPES__

#include <stdint.h>

typedef uint32_t DataRange;
typedef uint16_t DataPosition;

typedef enum DATA_MODE{
  __u8__,
  __u16__,
  __u32__,

  __i8__,
  __i16__,
  __i32__,

  __float__,
}DATA_MODE;

typedef enum {
  UNSIGNED,
  SIGNED,
  FLOAT,
  STRING,
}DataFormat;

#endif // !__LOG_OBJ_TYPES__
