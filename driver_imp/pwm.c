#include "component.h"

#include "Ifx_Types.h"
#include "IfxGpt12.h"
#include "IfxPort.h"
#include "IfxGtm_Atom_Pwm.h"

#include <stdint.h>
#include <stddef.h>

#define ISR_PRIORITY_ATOM   20                                  /* Interrupt priority number                        */
#define CLK_FREQ            1000000.0f                          /* CMU clock frequency, in Hertz                    */
#define PWM_PERIOD          10000                                /* PWM period for the ATOM, in ticks                */
#define PERCENTAGE PWM_PERIOD / 100                    /* PWM duty cycle for the ATOM                      */

static struct{
  const IfxGtm_Atom_ToutMap* const m_pin;
  IfxGtm_Atom_Pwm_Config g_atomConfig;                            /* Timer configuration structure                    */
  IfxGtm_Atom_Pwm_Driver g_atomDriver;                            /* Timer Driver structure                           */
}PWM_INFO[__NUM_OF_PWM_GPIOS__] =
{
  {.m_pin = &IfxGtm_ATOM2_3_TOUT97_P11_6_OUT}, //INFO: PWM_GPIO_ASSI_LIGHT_BLU (0)
  {.m_pin = &IfxGtm_ATOM2_6_TOUT100_P11_11_OUT}, //INFO: PWM_GPIO_ASSI_LIGHT_YELLOW (1)
  {.m_pin = &IfxGtm_ATOM0_6N_TOUT27_P33_5_OUT}, //INFO: PWM_GPIO_ASSI_SOUND (2)
  {.m_pin = &IfxGtm_ATOM0_4_TOUT14_P00_5_OUT}, //INFO: PWM_DEBUG_LED (3)
  {.m_pin = &IfxGtm_ATOM0_5_TOUT15_P00_6_OUT}, //INF: PWM_DEBUG_LED_1(4)
};

struct GpioPwm_t{
  enum GPIO_PWM_PIN pwm_id:8;
  const uint8_t padding[15];
};

union GpioPwm_h_t_conv{
  GpioPwm_h* const hidden;
  struct GpioPwm_t* const clear;
};

#ifdef DEBUG
char __assert_size_pwm[(sizeof(GpioPwm_h)==sizeof(struct GpioPwm_t)?1:-1)];
char __assert_align_pwm[(RACEUP_ALIGNOF(GpioPwm_h)==RACEUP_ALIGNOF(struct GpioPwm_t)?1:-1)];
#endif

int8_t hardware_init_gpio_pwm(GpioPwm_h* const restrict self, const enum GPIO_PWM_PIN id)
{
  union GpioPwm_h_t_conv conv = {self};
  struct GpioPwm_t* const p_self = conv.clear;
  IfxGtm_Atom_Pwm_Config* g_atomConfig = &PWM_INFO[id].g_atomConfig;
  IfxGtm_Atom_Pwm_Driver* g_atom_driver = &PWM_INFO[id].g_atomDriver;
  IfxGtm_Atom_ToutMap* pin = PWM_INFO[id].m_pin;

  p_self->pwm_id = id;

  IfxGtm_enable(&MODULE_GTM);                                             /* Enable GTM                           */

  IfxGtm_Cmu_enableClocks(&MODULE_GTM, IFXGTM_CMU_CLKEN_CLK0);            /* Enable the CMU clock 0 + id               */
  IfxGtm_Atom_Pwm_initConfig(g_atomConfig, &MODULE_GTM);                 /* Initialize default parameters        */

  g_atomConfig->atom = pin->atom;                                       /* Select the ATOM depending on the LED     */
  g_atomConfig->atomChannel = pin->channel;                             /* Select the channel depending on the LED  */
  g_atomConfig->period = 5000;                                   /* Set timer period                         */
  g_atomConfig->pin.outputPin = pin;                                  /* Set LED as output                        */
  g_atomConfig->synchronousUpdateEnabled = TRUE;                       /* Enable synchronous update                */

  IfxGtm_Atom_Pwm_init(g_atom_driver, g_atomConfig);                 /* Initialize the PWM                       */
  IfxGtm_Atom_Pwm_start(g_atom_driver, TRUE);                         /* Start the PWM                            */

  return 0;
}

int8_t hardware_write_gpio_pwm(GpioPwm_h* const restrict self, const uint16_t duty_cycle)
{
  union GpioPwm_h_t_conv conv = {self};
  struct GpioPwm_t* const p_self = conv.clear;

  // const uint32_t step = PWM_INFO[p_self->pwm_id].g_atomConfig.period/100;
  const uint32_t step = 5000/100;

  PWM_INFO[p_self->pwm_id].g_atomConfig.dutyCycle = duty_cycle * step;
  IfxGtm_Atom_Pwm_init(&PWM_INFO[p_self->pwm_id].g_atomDriver, &PWM_INFO[p_self->pwm_id].g_atomConfig);
  return 0;
}
