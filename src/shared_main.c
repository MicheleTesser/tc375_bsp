/**************************************************************************************************
 * FILE: shared_main.c
 *
 * DESCRIPTION:
 *
 *    Application main file supporting multicore execution implemented as shared
 *code.
 *
 * FILE HISTORY:
 *
 *    V1  06.2020  RO
 *    - Base reference example
 *
 **************************************************************************************************
 * Copyright (C) 2015-2020 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 *************************************************************************************************/

/* default example BSP package */
#include "adc.h"

/* set of BSP API implementation by means of iLLD drivers */
#include "Ifx_Lwip.h"
#include "can.h"
#include "component.h"
#include "gpio.h"
#include "Configuration.h"
#include "IfxCpu.h"
#include "IfxCpu_Intrinsics.h"
#include "IfxCpu_reg.h"
#include "IfxScuCcu.h"
#include "IfxScuWdt.h"
#include "serial.h"
#include <stdint.h>

extern unsigned int __INTTAB_CPU0[];
extern unsigned int __INTTAB_CPU1[];
extern unsigned int __INTTAB_CPU2[];

IFX_STATIC IFX_CONST IfxScuCcu_FlashWaitstateConfig IfxScuCcu_defaultFlashWaitstateConfig = IFXSCU_CFG_FLASH_WAITSTATE;

IFX_STATIC IFX_CONST IfxScuCcu_PllStepConfig IfxScuCcu_defaultPllConfigSteps[] = {
    IFXSCU_CFG_PLL_STEPS
};

IFX_CONST IfxScuCcu_Config clockConfig = {
    IFXSCU_CFG_PLL_INITIAL_STEP,
    {
        (uint8)(sizeof(IfxScuCcu_defaultPllConfigSteps) / sizeof(IfxScuCcu_PllStepConfig)),
        IfxScuCcu_defaultPllConfigSteps
    },
    IFXSCU_CFG_CLK_DISTRIBUTION,
    &IfxScuCcu_defaultFlashWaitstateConfig,
    &IfxScuCcu_defaultModConfig
};

extern void _crt0_reset(void);

static void startAllCores(void)
{
  uint32_t currentCore = IfxCpu_getCoreIndex();

#if (IFXCPU_NUM_MODULES > 1)
  if (currentCore != 1U) {
    IfxCpu_startCore(&MODULE_CPU1, (uint32_t)&_crt0_reset);
  }
#endif
#if (IFXCPU_NUM_MODULES > 2)
  if (currentCore != 2U) {
    IfxCpu_startCore(&MODULE_CPU2, (uint32_t)&_crt0_reset);
  }
#endif
}

static void setBiv(uint32_t table)
{
  uint16_t pw = IfxScuWdt_getCpuWatchdogPassword();
  uint32_t biv = ((uint32_t)0xFFFFFFFEU & table);

  IfxScuWdt_clearCpuEndinit(pw);
  __mtcr(CPU_BIV, biv);
  IfxScuWdt_setCpuEndinit(pw);
  __isync();
}

static void printShowdownResult(const char *name, int8_t status)
{
  serial_write_raw("[CORE0][SHOWDOWN] ");
  serial_write_raw(name);
  serial_write_raw(": ");
  serial_write_str((status == 0) ? "OK" : "FAIL");
}

static void core0_component_showdown(void)
{
  int8_t status = 0;
  uint8_t passed = 0;
  uint8_t total = 0;

  total++;
  status = serial_setup(115200);
  if (status == 0) {
    passed++;
    serial_write_str("[CORE0][SHOWDOWN] START");
  }
  printShowdownResult("serial_setup", status);

  Gpio_h core0_led = {0};
  total++;
  status = hardware_init_gpio(&core0_led, GPIO_CORE_0_ALIVE_BLINK, 0);
  if (status == 0) {
    gpio_toggle(&core0_led);
    passed++;
  }
  printShowdownResult("gpio_core0", status);

  timer_init();
  total++;
  {
    time_var_microseconds start = timer_time_now();
    time_var_microseconds wait_ticks = get_tick_from_millis(1U);
    timer_wait(wait_ticks);
    time_var_microseconds end = timer_time_now();
    status = (end > start) ? 0 : -1;
  }
  if (status == 0) {
    passed++;
  }
  printShowdownResult("timer", status);

  Adc_h adc_front = {0};
  total++;
  status = ADC_init(&adc_front, ADC_PIN_BRAKE_PRESSURE_FRONT);
  if (status == 0) {
    (void)ADC_read(&adc_front);
    ADC_destroy(&adc_front);
    passed++;
  }
  printShowdownResult("adc_front", status);

  GpioPwm_h pwm_debug = {0};
  total++;
  status = hardware_init_gpio_pwm(&pwm_debug, PWM_DEBUG_LED);
  if (status == 0) {
    (void)hardware_write_gpio_pwm(&pwm_debug, 25U);
    passed++;
  }
  printShowdownResult("pwm_debug_led", status);

  total++;
  status = hardware_init_can(CAN_DEBUG, _500_KBYTE_S_);
  if (status == 0) {
    struct CanNode *can_node = hardware_init_can_get_ref_node(CAN_DEBUG);
    if (can_node != NULL) {
      CanMessage mex = {0};
      mex.id = 0x123U;
      mex.message_size = 1U;
      mex.buffer[0] = 0x55U;
      status = hardware_write_can(can_node, &mex);
      hardware_init_can_destroy_ref_node(&can_node);
      if (status == 0) {
        passed++;
      }
    } else {
      status = -2;
    }
  }
  printShowdownResult("can_debug", status);

  total++;
  {
    EthernetNodeIpv4_h eth = {0};
    char ip_addr[] = "192.168.0.2";
    IpAddrIpV4Port remote = {ip_addr, 8000U};
    status = hardware_ethernet_udp_init(&eth, &remote);
    if (status == 0) {
      hardware_ethernet_udp_free(&eth);
      passed++;
    }
  }
  printShowdownResult("ethernet_udp", status);

  total++;
  status = hardware_init_flash_memory();
  if (status == 0) {
    FlashMemoryBank_h bank = {0};
    uint32_t words[TOT_NUM_OF_PARTS] = {0};
    status = flash_memory_req_bank(&bank, 0U);
    if (status == 0) {
      status = flash_memory_read_bank(&bank, words);
      (void)flash_memory_unleash_bank(&bank);
    }
    if (status == 0) {
      passed++;
    }
  }
  printShowdownResult("flash_read_bank0", status);

  serial_write_raw("[CORE0][SHOWDOWN] SUMMARY ");
  serial_write_uint8_t(passed);
  serial_write_raw("/");
  serial_write_uint8_t(total);
  serial_write_str(" components OK");
}

/* ================================================================================================
 * SYMBOLS
 * ==============================================================================================*/

//* iLLD style time period definition */
#define STM_FREQ() (IfxScuCcu_getStmFrequency())
#define STM_TIMER_BASE_PERIOD(ms) ((STM_FREQ() / 1000) * ms)

/* Time Base Period - each Core sets its period derived from this base */
#define TIMER_BASE_PERIOD STM_TIMER_BASE_PERIOD(250)

/* Timer Interrupt priority the same on each core */
#define TIMER_INTERRUPT_PRIO 10

/* ================================================================================================
 * FUNCTIONS
 * ==============================================================================================*/

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: shared_main()
 *     Shared entry point if there are more core present in uC derivative.
 *     It is called by crt0 startup code after crt0 finishes 'C' runtime
 * environment initialization. NOTES: To get a HW platform in an expected state,
 * the initialization is done within Crt0PostInit hook called before entering
 * shared_main from crt0.
 * ------------------------------------------------------------------------------------------------
 */
void shared_main(void) {
  uint32_t coreId = IfxCpu_getCoreIndex();
  if (coreId != 0) {
    for (;;) {
    }
  }
  IfxCpu_enableInterrupts();
  while(1){
    core0_component_showdown();
  }
}

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: Crt0PreInit()
 *     Crt0 Hook before memory clear & copy operation take place.
 *     The code cannot rely on initialized and cleared global data !
 * ------------------------------------------------------------------------------------------------
 */
void Crt0PreInit(void) {
  uint32_t coreId = IfxCpu_getCoreIndex();

  if (coreId == 0U) {
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
  }

  IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());

  IfxCpu_invalidateProgramCache();
  IfxCpu_setDataCache(0);
  IfxCpu_setProgramCache(1);
}

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: Crt0PostInit()
 *     Crt0 Hook after complete crt0 initialization, just before shared_main()
 * entry point is called. It is the place of uC shared HW resource
 * initialization.
 * ------------------------------------------------------------------------------------------------
 */
void Crt0PostInit(void) {
  uint32_t coreId = IfxCpu_getCoreIndex();

  if (coreId == 0U) {
    IfxScuCcu_init(&clockConfig);
    startAllCores();
  }

  switch (coreId) {
  case 0U:
    setBiv((uint32_t)__INTTAB_CPU0);
    break;
#if (IFXCPU_NUM_MODULES > 1)
  case 1U:
    setBiv((uint32_t)__INTTAB_CPU1);
    break;
#endif
#if (IFXCPU_NUM_MODULES > 2)
  case 2U:
    setBiv((uint32_t)__INTTAB_CPU2);
    break;
#endif
  default:
    break;
  }
}
