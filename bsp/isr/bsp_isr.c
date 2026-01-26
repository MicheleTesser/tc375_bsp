/**************************************************************************************************
 * FILE: bsp_isr.c
 *
 * UC ARCHITECTURE:
 *
 *    AURIX TC3xx
 *
 * UC FAMILY:
 *
 *    TC3xx
 *
 * DESCRIPTION:
 *
 *    Initialization of the Core's interrupt system (BIV register).
 *    Each core has its own interrupt vector table provided by this BSP.
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
 ************************************************************************************************/

/* include BSP API interface */
#include "bsp.h"

/* implement only when BSP provides HW Interrupt support */
#ifndef BSP_ISR_SUPPORT_DISABLED


/* ================================================================================================
 * Undefined handler implementation
 * ==============================================================================================*/

void isr_undef_handler(void)
{
	__asm ("debug");
	for (;;);
}


/* ================================================================================================
 * ISR Vector table for each uC core
 * ==============================================================================================*/

extern uint32_t* BSP_ISR_VECTOR_TABLE_CPU0;

#if (UC_NB_CORES > 1)
extern uint32_t* BSP_ISR_VECTOR_TABLE_CPU1;

#if (UC_NB_CORES > 2)
extern uint32_t* BSP_ISR_VECTOR_TABLE_CPU2;

#if (UC_NB_CORES > 3)
extern uint32_t* BSP_ISR_VECTOR_TABLE_CPU3;

#if (UC_NB_CORES > 4)
extern uint32_t* BSP_ISR_VECTOR_TABLE_CPU4;

#if (UC_NB_CORES > 5)
extern uint32_t* BSP_ISR_VECTOR_TABLE_CPU5;

#endif // CORE 5
#endif // CORE 4
#endif // CORE 3
#endif // CORE 2
#endif // CORE 1


/* ================================================================================================
 * BSP ISR Handlers table for each core
 * in case the RAM version is configured
 * ==============================================================================================*/

#pragma clang section bss=".BspIsrRamTable_Cpu0"
BspIsrHandler BspIsrRamTable_Cpu0[BSP_ISR_MAX_VECTORS];

#if (UC_NB_CORES > 1)
#pragma clang section bss=".BspIsrRamTable_Cpu1"
BspIsrHandler BspIsrRamTable_Cpu1[BSP_ISR_MAX_VECTORS];

#if (UC_NB_CORES > 2)
#pragma clang section bss=".BspIsrRamTable_Cpu2"
BspIsrHandler BspIsrRamTable_Cpu2[BSP_ISR_MAX_VECTORS];

#if (UC_NB_CORES > 3)
#pragma clang section bss=".BspIsrRamTable_Cpu3"
BspIsrHandler BspIsrRamTable_Cpu3[BSP_ISR_MAX_VECTORS];

#if (UC_NB_CORES > 4)
#pragma clang section bss=".BspIsrRamTable_Cpu4"
BspIsrHandler BspIsrRamTable_Cpu4[BSP_ISR_MAX_VECTORS];

#if (UC_NB_CORES > 5)
#pragma clang section bss=".BspIsrRamTable_Cpu5"
BspIsrHandler BspIsrRamTable_Cpu5[BSP_ISR_MAX_VECTORS];

#endif // CORE 5
#endif // CORE 4
#endif // CORE 3
#endif // CORE 2
#endif // CORE 1


/* ================================================================================================
 * ISR API
 * ==============================================================================================*/

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: isr_Init
 *     It sets ISR Vector Table in the corresponding Core's BIV register.
 *     It initializes RAM ISR Table with all priority vectors pointing
 *     to Undefined Handler routine.
 * ------------------------------------------------------------------------------------------------
 */
void bsp_isr_Init(void)
{
	/* holds the vector table for the requested Core */
	uint32_t isr_vector_table;

	/* Get current Core logical number */
	uint32_t coreId = bsp_uc_core_GetCurrentCore();

	/* Select right vector table */
	switch (coreId)
	{
		case UC_CORE_MAIN_0:
			isr_vector_table = (uint32_t) &BSP_ISR_VECTOR_TABLE_CPU0;
			break;

#if (UC_NB_CORES > 1)
		case UC_CORE_MAIN_1:
			isr_vector_table = (uint32_t) &BSP_ISR_VECTOR_TABLE_CPU1;
			break;

#if (UC_NB_CORES > 2)
		case UC_CORE_MAIN_2:
			isr_vector_table = (uint32_t) &BSP_ISR_VECTOR_TABLE_CPU2;
			break;

#if (UC_NB_CORES > 3)
		case UC_CORE_MAIN_3:
			isr_vector_table = (uint32_t) &BSP_ISR_VECTOR_TABLE_CPU3;
			break;

#if (UC_NB_CORES > 4)
		case UC_CORE_MAIN_4:
			isr_vector_table = (uint32_t) &BSP_ISR_VECTOR_TABLE_CPU4;
			break;

#if (UC_NB_CORES > 5)
		case UC_CORE_MAIN_5:
			isr_vector_table = (uint32_t) &BSP_ISR_VECTOR_TABLE_CPU5;
			break;

#endif // CORE 5
#endif // CORE 4
#endif // CORE 3
#endif // CORE 2
#endif // CORE 1

		default:
			return; // nothing to do in case of wrong coreId parameter
	}

	/* Register ISR Vector Table (dispatch table) to Core BIV register
	 * Set VSS = 0 to get expected 32B spacing in between vectors
	 */
	bsp_uc_intc_SetBIV(isr_vector_table, 0);

	/* Initialize RAM Handler table entries */
	uint32_t i;
	for (i=0; i < BSP_ISR_MAX_VECTORS; i++)
		bsp_isr_RegisterHandler(coreId, (BspIsrHandler) isr_undef_handler, i);
}


/* ------------------------------------------------------------------------------------------------
 * Function: isr_RegisterHandler
 *    it stores the ISR Handler coordinates in RAM table in case of
 *    selected RAM mode, otherwise does nothing
 * ------------------------------------------------------------------------------------------------
 */
void bsp_isr_RegisterHandler(uint32_t coreId, BspIsrHandler handler, uint32_t prio)
{
	/* holds RAM Handler table for requested core */
	BspIsrHandler *handler_table;

	/* Select right vector table */
	switch (coreId)
	{
		case UC_CORE_MAIN_0:
			handler_table = (BspIsrHandler *) BspIsrRamTable_Cpu0;
			break;

#if (UC_NB_CORES > 1)
		case UC_CORE_MAIN_1:
			handler_table = (BspIsrHandler *) BspIsrRamTable_Cpu1;
			break;

#if (UC_NB_CORES > 2)
		case UC_CORE_MAIN_2:
			handler_table = (BspIsrHandler *) BspIsrRamTable_Cpu2;
			break;

#if (UC_NB_CORES > 3)
		case UC_CORE_MAIN_3:
			handler_table = (BspIsrHandler *) BspIsrRamTable_Cpu3;
			break;

#if (UC_NB_CORES > 4)
		case UC_CORE_MAIN_4:
			handler_table = (BspIsrHandler *) BspIsrRamTable_Cpu4;
			break;

#if (UC_NB_CORES > 5)
		case UC_CORE_MAIN_5:
			handler_table = (BspIsrHandler *) BspIsrRamTable_Cpu5;
			break;

#endif // CORE 5
#endif // CORE 4
#endif // CORE 3
#endif // CORE 2
#endif // CORE 1

		default:
			return; // nothing to do in case of wrong coreId parameter
	}

	/* Update table entry, only for supported priorities,
	 * otherwise nothing happens
	 */
	if (prio < BSP_ISR_MAX_VECTORS)
		handler_table[prio] = handler;
}


#endif /* #ifndef BSP_ISR_SUPPORT_DISABLED */
