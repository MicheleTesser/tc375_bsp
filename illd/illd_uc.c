/******************************************************************************
 * FILE
 *    illd_uc.c
 *
 * UC ARCHITECTURE:
 *    TC3x
 *
 * UC FAMILY:
 *    TC39x
 *
 * DESCRIPTION
 *    uC Low Level supportive routines.
 *
 * FILE HISTORY:
 *    V1    06.2020    RO
 *        > Initial version for AURIX
 *
 ******************************************************************************
 * Copyright (C) 2015-2020 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 *****************************************************************************/

/* BSP support package API and types
 * - path is set in the Eclipse project C settings */
#include "bsp.h"

/* iLLD package API and types */
#include "illd.h"

/* ============================================================================
 * LLD DEFINES
 * ==========================================================================*/

//* LED port mapping */
#define LED_PORT(port) (illd_uc_get_led_port(port))

#ifndef NULL_PTR
#define NULL_PTR	0
#endif /* NULL_PTR */

/* ============================================================================
 * LOCAL DATA
 * ==========================================================================*/

/*
 * Clock configuration taken from iLLD for TC3xx Demos
 */

IFX_STATIC IFX_CONST IfxScuCcu_FlashWaitstateConfig IfxScuCcu_defaultFlashWaitstateConfig = IFXSCU_CFG_FLASH_WAITSTATE;

IFX_STATIC IFX_CONST IfxScuCcu_PllStepConfig  IfxScuCcu_defaultPllConfigSteps[] =
{
    IFXSCU_CFG_PLL_STEPS
};

IFX_CONST IfxScuCcu_Config  clockConfig =
{
    IFXSCU_CFG_PLL_INITIAL_STEP,
    {
        (uint8) (sizeof(IfxScuCcu_defaultPllConfigSteps) / sizeof(IfxScuCcu_PllStepConfig)),
        IfxScuCcu_defaultPllConfigSteps
    },
    IFXSCU_CFG_CLK_DISTRIBUTION,
    &IfxScuCcu_defaultFlashWaitstateConfig,
    &IfxScuCcu_defaultModConfig
};


/* ============================================================================
 * LLD API
 * ==========================================================================*/

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_get_led_port()
 *    Mapping of the port Pins connecting to LEDs on board.
 * --------------------------------------------------------------------------*/
Ifx_P * illd_uc_get_led_port(uint32_t board_led_port)
{
	Ifx_P * port;
		switch (board_led_port)
	{
		case 0:
			port =  &MODULE_P00;
			break;
		
		case 1:
			port =  &MODULE_P01;
			break;
		
		case 2:
			port =  &MODULE_P02;
			break;
		
		case 10:
			port =  &MODULE_P10;
			break;
		
		case 11:
			port =  &MODULE_P11;
			break;
		
		case 12:
			port =  &MODULE_P12;
			break;
		
		case 13:
			port =  &MODULE_P13;
			break;
		
		case 14:
			port =  &MODULE_P14;
			break;
		
		case 15:
			port =  &MODULE_P15;
			break;
		
		case 20:
			port =  &MODULE_P20;
			break;
		
		case 21:
			port =  &MODULE_P21;
			break;
		
		case 22:
			port =  &MODULE_P22;
			break;
		
		case 23:
			port =  &MODULE_P23;
			break;
		
		case 32:
			port =  &MODULE_P32;
			break;
		
		case 33:
			port =  &MODULE_P33;
			break;
		
		case 34:
			port =  &MODULE_P34;
			break;
		
		case 40:
			port =  &MODULE_P40;
			break;

		default:
			port = NULL_PTR;
			break;
	}
	
	return port;
}


/* ----------------------------------------------------------------------------
 * FUNCTION: illd_get_ifx_led_state()
 *    Convert the BSP led pin state to Ifx type
 * --------------------------------------------------------------------------*/
IfxPort_State illd_get_ifx_led_State(BOARD_LED_SET_e set)
{
	IfxPort_State new_set = IfxPort_State_notChanged;

	switch (set) {
		case BOARD_LED_SET_NO_CHANGE:
			new_set = IfxPort_State_notChanged; //(0 << 16) | (0 << 0);  /**< \brief Ifx_P pin is left unchanged. */

			break;

		case BOARD_LED_SET_OFF:
			new_set = IfxPort_State_high; //(0 << 16) | (1U << 0); /**< \brief Ifx_P pin is set to high. */

			break;

		case BOARD_LED_SET_ON:
			new_set = IfxPort_State_low; //(1U << 16) | (0 << 0); /**< \brief Ifx_P pin is set to low. */

			break;

		case BOARD_LED_SET_TOGGLE:
			new_set = IfxPort_State_toggled; //(1U << 16) | (1U << 0); /**< \brief Ifx_P pin is toggled. */

			break;
		default:
			break;
	}

	return new_set;
}


/* ----------------------------------------------------------------------------
 * FUNCTION: illd_board_led_InitAll()
 *    The initialization of the port Pins connecting to LEDs on board.
 *    Pins are initialized to output mode with a required value.
 * --------------------------------------------------------------------------*/
void illd_board_led_InitAll(BOARD_LED_SET_e set)
{
	int i;

	for (i = 0; i < BOARD_NB_LEDS; i++)
	{
		/* set output before output enable */
		IfxPort_setPinMode(LED_PORT(board_led[i].port), board_led[i].index, IfxPort_Mode_outputPushPullGeneral);

		/* enable pin output mode */
		IfxPort_setPinModeOutput(LED_PORT(board_led[i].port), board_led[i].index, IfxPort_OutputMode_pushPull, IfxPort_OutputIdx_general);

		/* Configure drive strength */
		IfxPort_setPinPadDriver(LED_PORT(board_led[i].port), board_led[i].index, IfxPort_PadDriver_cmosAutomotiveSpeed1);

		/* Set LED OFF */
		IfxPort_setPinState(LED_PORT(board_led[i].port), board_led[i].index, illd_get_ifx_led_State(set));
	}
}

/* ----------------------------------------------------------------------------
 * FUNCTION: illd_board_led_Set()
 *    Set output value using predefined modes
 * --------------------------------------------------------------------------*/
void illd_board_led_Set(BOARD_LED_e led, BOARD_LED_SET_e set)
{
	/* Set pin output */
	IfxPort_setPinState(LED_PORT(board_led[led].port), board_led[led].index, illd_get_ifx_led_State(set));
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_core_StartAllCores()
 *    Starts all inactive cores on multi-core uC derivative.
 *    Active ones are left untouched.
 *    All inactive cores will start at _crt0_reset entry point.
 * NOTE:
 *    All cores share the same crt0 startup routine.
 * --------------------------------------------------------------------------*/
extern void _crt0_reset(void);
void illd_uc_core_StartAllCores(void)
{
	uint32_t i;
	Ifx_CPU *cpu;

	/* start all cores, current core is left untouched to prevent TRAP
	 * during SYSCON register read without unlocking SafetyEndInit
	 */
	for (i = 0; i < UC_NB_CORES; i++)
	{
		uint32_t phyCoreId = (i == 5) ? 6 : i;

		if ((IfxCpu_Id) i == IfxCpu_getCoreId())
			continue;

		cpu = (Ifx_CPU *)((uint32_t)&MODULE_CPU0 + (uint32_t)(phyCoreId)*0x20000);
		IfxCpu_startCore(cpu, (uint32)&_crt0_reset);
	}
}

/* ----------------------------------------------------------------------------
 * FUNCTION: illd_uc_core_EnableICache()
 *    Enables Instruction cache on the current core
 * --------------------------------------------------------------------------*/
void illd_uc_core_EnableICache(void)
{
	/* invalidate the program cache */
	IfxCpu_invalidateProgramCache();

	/* enable the data cache for the CPU */
	IfxCpu_setDataCache(0);

	/* enable the program cache for the CPU */
	IfxCpu_setProgramCache(1);
}

/* ----------------------------------------------------------------------------
 * FUNCTION: illd_uc_intc_stm_SetSRC()
 *    Configure Interrupt mode to use on given processor
 * INPUT:
 *    channel  : STM channel
 *    coreId   : which core will handle the interrupt
 *    mode     : 0/1 = SW/HW
 * --------------------------------------------------------------------------*/
void illd_uc_intc_stm_SetSRC(uint32_t channel, uint32_t coreId, uint32_t prio) __attribute__((weak));
void illd_uc_intc_stm_SetSRC(uint32_t channel, uint32_t coreId, uint32_t prio)
{
	/* convert coreId to TOS needed by iLLD routine (see TC3x UM) */
    if (coreId != 0){
        coreId = coreId + 1;
    }

	/* set processor and priority values */
	IfxSrc_init(&MODULE_SRC.STM.STM[channel].SR[0], (IfxSrc_Tos) coreId, prio);

	/* ... and enable it */
	IfxSrc_enable(&MODULE_SRC.STM.STM[channel].SR[0]);
}

/* ----------------------------------------------------------------------------
 * FUNCTION: illd_uc_stm_ClearChannelIsrFlag()
 *    Clear active interrupt flag on requested PIT channel
 * INPUT:
 *    channel : STM module
 * --------------------------------------------------------------------------*/
void illd_uc_stm_ClearChannelIsrFlag(uint32_t channel)
{
	Ifx_STM * const  module_stm =  ((Ifx_STM*) ((uint32_t) &MODULE_STM0 + (0x100 * channel)));
	IfxStm_clearCompareFlag(module_stm, (IfxStm_Comparator) 0);
}

/* ----------------------------------------------------------------------------
 * FUNCTION: illd_uc_stm_ReloadChannel()
 *    Configures reload value.
 * INPUT:
 *    channel      : STM module
 *    reload_value : period in timer ticks (STM clock)
 * --------------------------------------------------------------------------*/
void illd_uc_stm_ReloadChannel(uint32_t channel, uint32_t reload_value)
{
	Ifx_STM * const  module_stm =  ((Ifx_STM*) ((uint32_t) &MODULE_STM0 + (0x100 * channel)));
	IfxStm_increaseCompare(module_stm, (IfxStm_Comparator) 0, reload_value);
}

/* ----------------------------------------------------------------------------
 * FUNCTION: illd_uc_core_GetCurrentCore()
 *    Reads value of the current execution Core calling this function
 * OUTPUT:
 *    uint32 : Core Id
 * --------------------------------------------------------------------------*/
uint32_t illd_uc_core_GetCurrentCore(void)
{
    return IfxCpu_getCoreId();
}

/* ----------------------------------------------------------------------------
 * FUNCTION: uc_stm_InitChannel()
 *    Configures reload value.
 * INPUT:
 *    channel      : STM module
 *    coreId	   : Core ID
 *    prio         : Interrupt priority
 *    reload_value : period in timer ticks (STM clock)
 * --------------------------------------------------------------------------*/
void illd_uc_stm_InitChannel(uint32_t channel, uint32_t coreId, uint32_t prio, uint32_t reload_value)
{
	Ifx_STM * const  module_stm =  ((Ifx_STM*) ((uint32_t) &MODULE_STM0 + (0x100 * channel)));
	IfxStm_CompareConfig stmConfig;

	/* convert coreId to TOS needed by iLLD routine (see TC3x UM) */
    if (coreId != 0){
        coreId = coreId + 1;
    }

	IfxStm_initCompareConfig(&stmConfig);
	stmConfig.ticks = reload_value;
	stmConfig.triggerPriority = prio;
	stmConfig.typeOfService = (IfxSrc_Tos) coreId;
	IfxStm_initCompare(module_stm, &stmConfig);
}

/* ----------------------------------------------------------------------------
 * FUNCTION: illd_IfxCpu_enableInterrupts()
 *    Enable global interrupts
 * --------------------------------------------------------------------------*/
void illd_IfxCpu_enableInterrupts(void)
{
	IfxCpu_enableInterrupts();
}

/* ----------------------------------------------------------------------------
 * FUNCTION: illd_uc_scu_wdt_DisableCpuWatchdog()
 *    Disables requested Safety Watchdog module.
 * INPUT:
 *    base : Requested SWT module base address in memory map
 * --------------------------------------------------------------------------*/
void illd_uc_scu_wdt_DisableCpuWatchdog(void)
{
	IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
}

/* ----------------------------------------------------------------------------
 * FUNCTION: illd_uc_scu_wdt_DisableSafetyWatchdog()
 *    Disables requested Safety Watchdog module.
 * INPUT:
 *    base : Requested SWT module base address in memory map
 * --------------------------------------------------------------------------*/
void illd_uc_scu_wdt_DisableSafetyWatchdog(void)
{
	IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
}

/* ----------------------------------------------------------------------------
 * FUNCTION: illd_uc_InitClock()
 *    It configures the uC clock system to the optimum setting
 *    for the max clock
 * --------------------------------------------------------------------------*/
void illd_uc_InitClock(void)
{
	IfxScuCcu_init(&clockConfig);
}
