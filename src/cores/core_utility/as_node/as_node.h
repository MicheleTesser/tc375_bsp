#ifndef __AS_NODE__
#define __AS_NODE__

#include <stdint.h>

typedef enum
{
  AS_NODE_OPEN =0,
  AS_NODE_CLOSED,
}AsNode_v;

typedef struct __attribute__((aligned(4)))
{
  const uint8_t private_data[8];
}AsNodeMut_h;


int8_t as_node_mut_init(AsNodeMut_h* const restrict self)__attribute__((__nonnull__(1)));

int8_t as_node_mut_open(AsNodeMut_h* const restrict self)__attribute__((__nonnull__(1)));
int8_t as_node_mut_close(AsNodeMut_h* const restrict self)__attribute__((__nonnull__(1)));

int8_t as_node_mut_destroy(AsNodeMut_h* const restrict self)__attribute__((__nonnull__(1)));

AsNode_v as_node_read_get(void);

#endif // !__AS_NODE__
