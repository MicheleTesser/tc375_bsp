#include "rtd_assi_sound.h"
#include "../../../lib/raceup_board/raceup_board.h"
#include <stdatomic.h>
#include <stdint.h>
#include <string.h>


struct RtdAssiSound_t{
  uint8_t m_started;
  AssiMetadata m_metadata;
};

union RtdAssiSound_h_t_conv {
  RtdAssiSound_h* const hidden;
  struct RtdAssiSound_t* const clear;
};

struct{
  _Atomic uint8_t m_init_done;
  _Atomic uint8_t m_started;
  GpioPwm_h m_rtd_assi_sound;
  time_var_microseconds pwm_timer;
  time_var_microseconds start_sound;
  AssiMetadata m_metadata;
}GPIO_RTD_ASSI;

#ifdef DEBUG
char __assert_size_rtd_assi_sound[(sizeof(RtdAssiSound_h)==sizeof(struct RtdAssiSound_t))?+1:-1];
char __assert_align_rtd_assi_sound[(_Alignof(RtdAssiSound_h)==_Alignof(struct RtdAssiSound_t))?+1:-1];

#endif /* ifdef DEBUG */

static void _sound_stop(void)
{
  atomic_store(&GPIO_RTD_ASSI.m_started, 0);
  GPIO_RTD_ASSI.start_sound =0;
  memset(&GPIO_RTD_ASSI.m_metadata, 0, sizeof(GPIO_RTD_ASSI.m_metadata));
  hardware_write_gpio_pwm(&GPIO_RTD_ASSI.m_rtd_assi_sound, 0);
}

static uint8_t _compute_duty_cicle(void)
{
  const time_var_microseconds time_passed = timer_time_now() - GPIO_RTD_ASSI.pwm_timer;
  const time_var_microseconds point_in_period = time_passed % GPIO_RTD_ASSI.m_metadata.duty_cycle_step*2;
  const uint8_t down_or_up = point_in_period > GPIO_RTD_ASSI.m_metadata.duty_cycle_step;
  return
    (GPIO_RTD_ASSI.m_metadata.duty_cycle_min * down_or_up) 
    +
    (GPIO_RTD_ASSI.m_metadata.duty_cycle_max * !down_or_up);
}

//public

int8_t NS_ASSIS_SOUND(init)(RtdAssiSound_h* const restrict self, const AssiMetadata* metadata)
{
  union RtdAssiSound_h_t_conv conv = {self};
  struct RtdAssiSound_t* const p_self = conv.clear;

  if (!atomic_exchange(&GPIO_RTD_ASSI.m_init_done, 1U))
  {
    hardware_init_gpio_pwm(&GPIO_RTD_ASSI.m_rtd_assi_sound, PWM_GPIO_ASSI_SOUND);
    hardware_write_gpio_pwm(&GPIO_RTD_ASSI.m_rtd_assi_sound, 0);
  }
  p_self->m_started=0;
  p_self->m_metadata = *metadata;
  return 0;
}

void NS_ASSIS_SOUND(update)(void)
{
  if(atomic_load(&GPIO_RTD_ASSI.m_started) &&
      (timer_time_now() - GPIO_RTD_ASSI.start_sound) <= GPIO_RTD_ASSI.m_metadata.duration)
  {
    const uint8_t duty_cycle = _compute_duty_cicle();
    hardware_write_gpio_pwm(&GPIO_RTD_ASSI.m_rtd_assi_sound, duty_cycle);
  }
  else
  {
    _sound_stop();
  }

}

int8_t NS_ASSIS_SOUND(start)(RtdAssiSound_h* const restrict self)
{
  union RtdAssiSound_h_t_conv conv = {self};
  struct RtdAssiSound_t* const p_self = conv.clear;

  if (!p_self->m_started && !atomic_load(&GPIO_RTD_ASSI.m_started))
  {
    memcpy(&GPIO_RTD_ASSI.m_metadata, &p_self->m_metadata, sizeof(GPIO_RTD_ASSI.m_metadata));
    atomic_store(&GPIO_RTD_ASSI.m_started, 1);
    p_self->m_started = 1;
    GPIO_RTD_ASSI.start_sound = timer_time_now();
    return 1;
  }
  return 0;
}

int8_t NS_ASSIS_SOUND(stop)(RtdAssiSound_h* const restrict self)
{
  union RtdAssiSound_h_t_conv conv = {self};
  struct RtdAssiSound_t* const p_self = conv.clear;

  if (p_self->m_started)
  {
    _sound_stop();
    p_self->m_started=0;
    return 1;
  }

  return 0;
}

int8_t NS_ASSIS_SOUND(destroy)(RtdAssiSound_h* const restrict self)
{
  memset(self, 0, sizeof(*self));
  return 0;
}
