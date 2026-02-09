#include "./emergency_module.h"
#include "../../../lib/raceup_board/raceup_board.h"
#include "../errno_trace/errno_trace.h"
#include "../car_speed/car_speed.h"
#include <stdatomic.h>
#include <stdint.h>
#include <string.h>

//private

#define NUM_EMERGENCY_BUFFER 8
#define MAX_SPEED_FOR_EMERGENCY (5.0f)

struct EmergencyNode_t
{
  uint8_t emergency_buffer[NUM_EMERGENCY_BUFFER];
  uint32_t emergency_counter;
};

union EmergencyNode_h_t_conv
{
  EmergencyNode_h* const restrict hidden;
  struct EmergencyNode_t* const restrict clear;
};

union EmergencyNode_h_t_conv_const
{
  const EmergencyNode_h* const restrict hidden;
  const struct EmergencyNode_t* const restrict clear;
};

static struct{
  _Atomic uint8_t lock;
  Gpio_h gpio_scs;
  uint8_t excepion_counter;
  uint8_t init_done:1;
}EXCEPTION_COUNTER;

static inline void _hw_raise_emergency(void)
{
  while (atomic_exchange(&EXCEPTION_COUNTER.lock, 1U));
  gpio_set_low(&EXCEPTION_COUNTER.gpio_scs);
  atomic_store(&EXCEPTION_COUNTER.lock, 0U);
}

static void _increase_global_emergency_counter(void) 
{
  while (atomic_exchange(&EXCEPTION_COUNTER.lock, 1U));
  EXCEPTION_COUNTER.excepion_counter++;
  atomic_store(&EXCEPTION_COUNTER.lock, 0U);
}

static void _solved_module_exception_state(void)__attribute__((__unused__));
static void _solved_module_exception_state(void)
{
  while (atomic_exchange(&EXCEPTION_COUNTER.lock, 1U));
  EXCEPTION_COUNTER.excepion_counter--;
  if (!EXCEPTION_COUNTER.excepion_counter)
  {
    gpio_set_high(&EXCEPTION_COUNTER.gpio_scs);
  }
  atomic_store(&EXCEPTION_COUNTER.lock, 0U);
}

static uint8_t read_globla_emergency_couner(void)__attribute__((__unused__));
static uint8_t read_globla_emergency_couner(void)
{
  while (atomic_exchange(&EXCEPTION_COUNTER.lock, 1U));
  const uint8_t res= EXCEPTION_COUNTER.excepion_counter;
  atomic_store(&EXCEPTION_COUNTER.lock, 0U);

  return res;
}

#ifdef DEBUG
uint8_t __assert_size_emergency_node[(sizeof(EmergencyNode_h)==sizeof(struct EmergencyNode_t))?1:-1];
uint8_t __assert_align_emergency_node[(_Alignof(EmergencyNode_h)==_Alignof(struct EmergencyNode_t))?1:-1];
#endif /* ifdef DEBUG */

//public

int8_t EmergencyNode_class_init(void)
{
  if (EXCEPTION_COUNTER.init_done || hardware_init_gpio(&EXCEPTION_COUNTER.gpio_scs, GPIO_SCS, 0)<0)
  {
    SET_TRACE(CORE_0, INIT_ERROR);
    return -1;   
  }
  gpio_set_high(&EXCEPTION_COUNTER.gpio_scs);
  EXCEPTION_COUNTER.init_done=1;
  EXCEPTION_COUNTER.excepion_counter=0;

  return 0;
}

int8_t EmergencyNode_init(EmergencyNode_h* const restrict self)
{
  union EmergencyNode_h_t_conv conv = {self};
  struct EmergencyNode_t* const restrict p_self =conv.clear;

  memset(p_self, 0, sizeof(*p_self));

  return 0;
}

int8_t EmergencyNode_raise(EmergencyNode_h* const restrict self, const uint8_t exeception)
{
  const uint8_t exception_byte = exeception/8;
  const uint8_t exception_bit = exeception % 8;
  union EmergencyNode_h_t_conv conv = {self};
  struct EmergencyNode_t* const restrict p_self =conv.clear;

  if (exeception >= NUM_EMERGENCY_BUFFER*8)
  {
    SET_TRACE(CORE_0, LIMIT_BUFFER_EXCEDED);
    return -1;
  }

  const uint8_t old_emergency_bit = (p_self->emergency_buffer[exception_byte] >> exception_bit) & 0x01;
  p_self->emergency_buffer[exception_byte] |= 1 << exception_bit;

  if (!old_emergency_bit)
  {
    if(!p_self->emergency_counter) {
      _increase_global_emergency_counter();
    }
    p_self->emergency_counter++;
  }

  if (car_speed_get() < MAX_SPEED_FOR_EMERGENCY)
  {
    _hw_raise_emergency();
  }

  return 0;
}

int8_t EmergencyNode_solve(EmergencyNode_h* const restrict self, const uint8_t exeception)
{
  const uint8_t exception_byte = exeception/8;
  const uint8_t exception_bit = exeception % 8;
  union EmergencyNode_h_t_conv conv = {self};
  struct EmergencyNode_t* const restrict p_self =conv.clear;

  if (exeception >= NUM_EMERGENCY_BUFFER * 8)
  {
    SET_TRACE(CORE_0, LIMIT_BUFFER_EXCEDED);
    return -1;
  }

  if (p_self->emergency_buffer[exception_byte] &  (1 << exception_bit))
  {
    p_self->emergency_buffer[exception_byte] ^= (1 << exception_bit);
    p_self->emergency_counter--;
    if (!p_self->emergency_counter)
    {
      _solved_module_exception_state();
    }
  }

  return 0;
}

int8_t EmergencyNode_is_emergency_state(const EmergencyNode_h* const restrict self)
{
  const union EmergencyNode_h_t_conv_const conv = {self};
  const struct EmergencyNode_t* const restrict p_self =conv.clear;

  return p_self->emergency_counter || read_globla_emergency_couner();
}

int8_t EmergencyNode_destroy(EmergencyNode_h* const restrict self)
{
  union EmergencyNode_h_t_conv conv = {self};
  struct EmergencyNode_t* restrict p_self =conv.clear;

  if (p_self->emergency_counter)
  {
    _solved_module_exception_state();
  }

  memset(self, 0, sizeof(*self));
  return 0;
}
