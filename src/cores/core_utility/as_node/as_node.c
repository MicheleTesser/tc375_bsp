#include "as_node.h"
#include "../../../lib/raceup_board/raceup_board.h"

#include <stdatomic.h>
#include <stdint.h>
#include <string.h>

struct AsNodeMut_t
{
  Gpio_h m_gpio_as_node;
};

union AsNodeMut_h_t_conv {
  AsNodeMut_h* const restrict hidden;
  struct AsNodeMut_t* const restrict clear;
};


#ifdef DEBUG
char __assert_size_as_node_mut[sizeof(AsNodeMut_h)==sizeof(struct AsNodeMut_t)?+1:-1];
char __assert_align_as_node_mut[_Alignof(AsNodeMut_h)==_Alignof(struct AsNodeMut_t)?+1:-1];
#endif /* ifdef DEBUG */

static struct{
  AsNode_v m_stat;
  _Atomic uint8_t m_taken;
}AS_NODE_STATUS;

//public

int8_t as_node_mut_init(AsNodeMut_h* const restrict self)
{
  union AsNodeMut_h_t_conv conv = {self};
  struct AsNodeMut_t* p_self = conv.clear;

  if (atomic_exchange(&AS_NODE_STATUS.m_taken, 1U))
  {
    return -1;
  }

  if(hardware_init_gpio(&p_self->m_gpio_as_node, GPIO_AS_NODE, 0)<0)
  {
    atomic_store(&AS_NODE_STATUS.m_taken, 0U);
    return -2;
  }

  as_node_mut_open(self);

  return 0;
}

int8_t as_node_mut_open(AsNodeMut_h* const restrict self)
{
  union AsNodeMut_h_t_conv conv = {self};
  struct AsNodeMut_t* p_self = conv.clear;

  gpio_set_low(&p_self->m_gpio_as_node);
  AS_NODE_STATUS.m_stat = 0;
  
  return 0;
}

int8_t as_node_mut_close(AsNodeMut_h* const restrict self)
{
  union AsNodeMut_h_t_conv conv = {self};
  struct AsNodeMut_t* p_self = conv.clear;

  gpio_set_high(&p_self->m_gpio_as_node);
  AS_NODE_STATUS.m_stat = 1;
  
  return 0;
}

AsNode_v as_node_read_get(void)
{
  return AS_NODE_STATUS.m_stat;
}


int8_t as_node_mut_destroy(AsNodeMut_h* const restrict self)
{
  union AsNodeMut_h_t_conv conv = {self};
  struct AsNodeMut_t* p_self = conv.clear;

  atomic_store(&AS_NODE_STATUS.m_taken, 0U);
  gpio_destroy(&p_self->m_gpio_as_node);

  memset(p_self, 0, sizeof(*self));

  return 0;
}
