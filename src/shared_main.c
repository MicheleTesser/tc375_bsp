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
#include "board/bsp_board.h"
#include "bsp.h"

/* set of BSP API implementation by means of iLLD drivers */
#include "Ifx_Lwip.h"
#include "can.h"
#include "component.h"
#include "gpio.h"
#include "illd.h"
#include "serial.h"
#include <stdint.h>

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
  /* Use BSP API to get current CoreId */
  uint32_t coreId = bsp_uc_core_GetCurrentCore();
  if (coreId != 0) {
    for (;;) {
    }
  }
  /* Enable External interrupts in the current Core */
  illd_IfxCpu_enableInterrupts();

  /* Forever empty loop - All activity executed in periodic interrupts on each
   * core */

  serial_setup(115200);
  Gpio_h gpio = {0};
  Gpio_h gpio_2 = {0};
  hardware_init_gpio(&gpio, GPIO_CORE_0_ALIVE_BLINK, 0);
  hardware_init_gpio(&gpio_2, GPIO_CORE_1_ALIVE_BLINK, 0);
  gpio_toggle(&gpio);

  for (;;) {
    serial_write_str("ciao mondo!");
    gpio_toggle(&gpio_2);
    gpio_toggle(&gpio);

    for (uint32_t i = 0; i < 100000000U; ++i) {
      __asm("nop");
    }
  }
}

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: Crt0PreInit()
 *     Crt0 Hook before memory clear & copy operation take place.
 *     The code cannot rely on initialized and cleared global data !
 * ------------------------------------------------------------------------------------------------
 */
void Crt0PreInit(void) {
  /* Use BSP API to get current CoreId */
  uint32_t coreId = bsp_uc_core_GetCurrentCore();

  /* disable Safety WATCHDOG by the RESET Core only */
  if (coreId == UC_RESET_CORE) {
    illd_uc_scu_wdt_DisableSafetyWatchdog();
  }

  /* disable Core WATCHDOG */
  illd_uc_scu_wdt_DisableCpuWatchdog();

  /* enable Core instruction cache */
  illd_uc_core_EnableICache();
}

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: Crt0PostInit()
 *     Crt0 Hook after complete crt0 initialization, just before shared_main()
 * entry point is called. It is the place of uC shared HW resource
 * initialization.
 * ------------------------------------------------------------------------------------------------
 */
void Crt0PostInit(void) {
  /* Use BSP API to get current CoreId */
  uint32_t coreId = bsp_uc_core_GetCurrentCore();

  /* Initialization of shared resources by RESET core */
  if (coreId == UC_RESET_CORE) {
    /* setup clock system of uC */
    illd_uc_InitClock();

    /* disable eventual external WATCHDOG on the board */
    bsp_board_wdg_Disable();

    /* switch off all supported LEDs on the board */
    illd_board_led_InitAll(BOARD_LED_SET_OFF);

    /* start other cores */
    illd_uc_core_StartAllCores();
  }

  /* setup ISR sub-system on each core */
  bsp_isr_Init();
}
