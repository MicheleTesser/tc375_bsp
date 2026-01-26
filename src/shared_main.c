/**************************************************************************************************
 * FILE: shared_main.c
 *
 * DESCRIPTION:
 *
 *    Application main file supporting multicore execution implemented as shared code.
 *
 * FILE HISTORY:
 *
 *    V1  05.2020  RO
 *    - Base reference example
 *
 **************************************************************************************************
 * Copyright (C) 2015-2020 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 *************************************************************************************************/


#include "bsp.h"
#include "FreeRTOS.h"
#include "task.h"


/* ================================================================================================
 * SYMBOLS
 * ==============================================================================================*/

/* Task periods in milliseconds */
#define LED_TASK_PERIOD_MS    250U

/* ================================================================================================
 * FUNCTIONS
 * ==============================================================================================*/

static void vLedTask(void *pvParameters)
{
	BOARD_LED_e led = (BOARD_LED_e)(uintptr_t)pvParameters;
	TickType_t lastWake = xTaskGetTickCount();
	TickType_t period = led * LED_TASK_PERIOD_MS;

	for (;;)
	{
		bsp_board_led_Set(led, BOARD_LED_SET_TOGGLE);
		vTaskDelayUntil(&lastWake, period);
	}
}

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: shared_main()
 *
 * Shared entry point if there are more core present in uC derivative.
 * It is called by crt0 startup code after crt0 finishes 'C' runtime environment initialization.
 *
 * To get a HW platform in an expected state, the initialization is done within Crt0PostInit
 * hook called before entering shared_main from crt0.
 * ------------------------------------------------------------------------------------------------
 */
void shared_main(void)
{
	/* Use BSP API to get current CoreId */
	uint32_t coreId = bsp_uc_core_GetCurrentCore();
	uint32_t ledIndex = coreId + 1U;

	configASSERT(xTaskCreate(vLedTask, "LED", configMINIMAL_STACK_SIZE,
	                         (void *)(uintptr_t)ledIndex, tskIDLE_PRIORITY + 2, NULL) == pdPASS);

	bsp_uc_intc_EnableExternalInterrupts();
	vTaskStartScheduler();

	for (;;);
}

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: Crt0PreInit()
 *
 *  Crt0 Hook before memory clear & copy operation take place.
 *  The code cannot rely on initialized and cleared global data !
 * ------------------------------------------------------------------------------------------------
 */
void Crt0PreInit(void)
{
	/* Use BSP API to get current CoreId */
	uint32_t coreId = bsp_uc_core_GetCurrentCore();

	/* disable Safety WATCHDOG by the RESET Core only */
	if (coreId == UC_RESET_CORE)
	{
		bsp_uc_scu_wdt_DisableSafetyWatchdog();
	}

	/* disable Core WATCHDOG */
	bsp_uc_scu_wdt_DisableCpuWatchdog();

	/* enable Core instruction cache */
	bsp_uc_core_EnableICache();
}

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: Crt0PostInit()
 *
 * Crt0 Hook after complete crt0 initialization, just before shared_main() entry point is called.
 * It is the place of uC shared HW resource initialization.
 * ------------------------------------------------------------------------------------------------
 */
void Crt0PostInit(void)
{
	/* Use BSP API to get current CoreId */
	uint32_t coreId = bsp_uc_core_GetCurrentCore();

	/* Initialization of shared resources by RESET core */
	if (coreId == UC_RESET_CORE)
	{
		/* setup clock system of uC */
		bsp_uc_InitClock();

		/* disable eventual external WATCHDOG on the board (not available on litekit)*/
#if !defined LITEKIT_TC375_V2
		bsp_board_wdg_Disable();
#endif

		/* switch off all supported LEDs on the board */
		bsp_board_led_InitAll(BOARD_LED_SET_OFF);

		/* start other cores */
		bsp_uc_core_StartAllCores();
	}

	/* setup ISR sub-system on each core */
	bsp_isr_Init();
}
