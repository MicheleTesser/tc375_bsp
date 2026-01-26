/**************************************************************************************************
 * FILE: bsp_isr_vector_table.c
 *
 * UC ARCHITECTURE:
 *
 *    AURIX TC3x
 *
 * UC FAMILY:
 *
 *    TC3xx
 *
 * DESCRIPTION:
 *
 *    Vector table for each core. Each entry represents jump to the handler
 *    that is expected to be declared with __attribute__ ((interrupt_handler))
 *    Location of the vector table is set in the linker file.
 *    Base of the vector is used to set Core's BIV register during the
 *    bsp_isr_Init function.
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

/* include common BSP header file */
#include "bsp.h"

/* implement only when BSP provides HW Interrupt support */
#ifndef BSP_ISR_SUPPORT_DISABLED


/* ================================================================================================
 * Vector table macros
 * ==============================================================================================*/

/* ISR Vector table definition macro
 * The Interrupt Vector Table requires 2kB alignment.
 * The table itself is wrapped in its section specific for each core.
 */
#define BSP_ISR_VECTOR_TABLE(i) \
    __asm (".section .bsp_isr_vector_table_cpu" #i " , \"ax\", @progbits"); \
    __asm (".align 11"); \
    __asm (".global BSP_ISR_VECTOR_TABLE_CPU" #i); \
    __asm ("BSP_ISR_VECTOR_TABLE_CPU" #i ":")

/* ISR Vector Handler X - prolog + jump to application handler
 * ISR Application Handlers are stored in core's RAM table to allow dynamic
 * interrupt handler assignment.
 */
#define BSP_ISR_DEFINE_HANDLER(cpu, i) \
    __asm (".align 5"); \
    __asm ("bisr " #i); \
    __asm ("movh.a %a14, hi:(BspIsrRamTable_Cpu" #cpu "+ 4 * " #i ")"); \
    __asm ("lea    %a14, [%a14]lo:(BspIsrRamTable_Cpu" #cpu "+ 4 * " #i ")"); \
    __asm ("ld.a   %a14, [%a14]0"); \
    __asm ("calli  %a14"); \
    __asm ("rslcx"); \
    __asm ("rfe");


/* ================================================================================================
 * ISR VECTOR TABLES definition
 * BSP supports (defines) only 32 priority vectors that should be sufficient for evaluation
 * purposes and simple test applications.
 * ==============================================================================================*/

/* ------------------------------------------------------------------------------------------------
 * CORE 0
 * ----------------------------------------------------------------------------------------------*/
#if (UC_NB_CORES > 0)

BSP_ISR_VECTOR_TABLE(0);

BSP_ISR_DEFINE_HANDLER(0,0);
BSP_ISR_DEFINE_HANDLER(0,1);
BSP_ISR_DEFINE_HANDLER(0,2);
BSP_ISR_DEFINE_HANDLER(0,3);
BSP_ISR_DEFINE_HANDLER(0,4);
BSP_ISR_DEFINE_HANDLER(0,5);
BSP_ISR_DEFINE_HANDLER(0,6);
BSP_ISR_DEFINE_HANDLER(0,7);
BSP_ISR_DEFINE_HANDLER(0,8);
BSP_ISR_DEFINE_HANDLER(0,9);

BSP_ISR_DEFINE_HANDLER(0,10);
BSP_ISR_DEFINE_HANDLER(0,11);
BSP_ISR_DEFINE_HANDLER(0,12);
BSP_ISR_DEFINE_HANDLER(0,13);
BSP_ISR_DEFINE_HANDLER(0,14);
BSP_ISR_DEFINE_HANDLER(0,15);
BSP_ISR_DEFINE_HANDLER(0,16);
BSP_ISR_DEFINE_HANDLER(0,17);
BSP_ISR_DEFINE_HANDLER(0,18);
BSP_ISR_DEFINE_HANDLER(0,19);

BSP_ISR_DEFINE_HANDLER(0,20);
BSP_ISR_DEFINE_HANDLER(0,21);
BSP_ISR_DEFINE_HANDLER(0,22);
BSP_ISR_DEFINE_HANDLER(0,23);
BSP_ISR_DEFINE_HANDLER(0,24);
BSP_ISR_DEFINE_HANDLER(0,25);
BSP_ISR_DEFINE_HANDLER(0,26);
BSP_ISR_DEFINE_HANDLER(0,27);
BSP_ISR_DEFINE_HANDLER(0,28);
BSP_ISR_DEFINE_HANDLER(0,29);

BSP_ISR_DEFINE_HANDLER(0,30);
BSP_ISR_DEFINE_HANDLER(0,31);

/* ------------------------------------------------------------------------------------------------
 * CORE 1
 * ----------------------------------------------------------------------------------------------*/
#if (UC_NB_CORES > 1)

BSP_ISR_VECTOR_TABLE(1);

BSP_ISR_DEFINE_HANDLER(1,0);
BSP_ISR_DEFINE_HANDLER(1,1);
BSP_ISR_DEFINE_HANDLER(1,2);
BSP_ISR_DEFINE_HANDLER(1,3);
BSP_ISR_DEFINE_HANDLER(1,4);
BSP_ISR_DEFINE_HANDLER(1,5);
BSP_ISR_DEFINE_HANDLER(1,6);
BSP_ISR_DEFINE_HANDLER(1,7);
BSP_ISR_DEFINE_HANDLER(1,8);
BSP_ISR_DEFINE_HANDLER(1,9);

BSP_ISR_DEFINE_HANDLER(1,10);
BSP_ISR_DEFINE_HANDLER(1,11);
BSP_ISR_DEFINE_HANDLER(1,12);
BSP_ISR_DEFINE_HANDLER(1,13);
BSP_ISR_DEFINE_HANDLER(1,14);
BSP_ISR_DEFINE_HANDLER(1,15);
BSP_ISR_DEFINE_HANDLER(1,16);
BSP_ISR_DEFINE_HANDLER(1,17);
BSP_ISR_DEFINE_HANDLER(1,18);
BSP_ISR_DEFINE_HANDLER(1,19);

BSP_ISR_DEFINE_HANDLER(1,20);
BSP_ISR_DEFINE_HANDLER(1,21);
BSP_ISR_DEFINE_HANDLER(1,22);
BSP_ISR_DEFINE_HANDLER(1,23);
BSP_ISR_DEFINE_HANDLER(1,24);
BSP_ISR_DEFINE_HANDLER(1,25);
BSP_ISR_DEFINE_HANDLER(1,26);
BSP_ISR_DEFINE_HANDLER(1,27);
BSP_ISR_DEFINE_HANDLER(1,28);
BSP_ISR_DEFINE_HANDLER(1,29);

BSP_ISR_DEFINE_HANDLER(1,30);
BSP_ISR_DEFINE_HANDLER(1,31);

/* ------------------------------------------------------------------------------------------------
 * CORE 2
 * ----------------------------------------------------------------------------------------------*/
#if (UC_NB_CORES > 2)

BSP_ISR_VECTOR_TABLE(2);

BSP_ISR_DEFINE_HANDLER(2,0);
BSP_ISR_DEFINE_HANDLER(2,1);
BSP_ISR_DEFINE_HANDLER(2,2);
BSP_ISR_DEFINE_HANDLER(2,3);
BSP_ISR_DEFINE_HANDLER(2,4);
BSP_ISR_DEFINE_HANDLER(2,5);
BSP_ISR_DEFINE_HANDLER(2,6);
BSP_ISR_DEFINE_HANDLER(2,7);
BSP_ISR_DEFINE_HANDLER(2,8);
BSP_ISR_DEFINE_HANDLER(2,9);

BSP_ISR_DEFINE_HANDLER(2,10);
BSP_ISR_DEFINE_HANDLER(2,11);
BSP_ISR_DEFINE_HANDLER(2,12);
BSP_ISR_DEFINE_HANDLER(2,13);
BSP_ISR_DEFINE_HANDLER(2,14);
BSP_ISR_DEFINE_HANDLER(2,15);
BSP_ISR_DEFINE_HANDLER(2,16);
BSP_ISR_DEFINE_HANDLER(2,17);
BSP_ISR_DEFINE_HANDLER(2,18);
BSP_ISR_DEFINE_HANDLER(2,19);

BSP_ISR_DEFINE_HANDLER(2,20);
BSP_ISR_DEFINE_HANDLER(2,21);
BSP_ISR_DEFINE_HANDLER(2,22);
BSP_ISR_DEFINE_HANDLER(2,23);
BSP_ISR_DEFINE_HANDLER(2,24);
BSP_ISR_DEFINE_HANDLER(2,25);
BSP_ISR_DEFINE_HANDLER(2,26);
BSP_ISR_DEFINE_HANDLER(2,27);
BSP_ISR_DEFINE_HANDLER(2,28);
BSP_ISR_DEFINE_HANDLER(2,29);

BSP_ISR_DEFINE_HANDLER(2,30);
BSP_ISR_DEFINE_HANDLER(2,31);

/* ------------------------------------------------------------------------------------------------
 * CORE 3
 * ----------------------------------------------------------------------------------------------*/
#if (UC_NB_CORES > 3)

BSP_ISR_VECTOR_TABLE(3);

BSP_ISR_DEFINE_HANDLER(3,0);
BSP_ISR_DEFINE_HANDLER(3,1);
BSP_ISR_DEFINE_HANDLER(3,2);
BSP_ISR_DEFINE_HANDLER(3,3);
BSP_ISR_DEFINE_HANDLER(3,4);
BSP_ISR_DEFINE_HANDLER(3,5);
BSP_ISR_DEFINE_HANDLER(3,6);
BSP_ISR_DEFINE_HANDLER(3,7);
BSP_ISR_DEFINE_HANDLER(3,8);
BSP_ISR_DEFINE_HANDLER(3,9);

BSP_ISR_DEFINE_HANDLER(3,10);
BSP_ISR_DEFINE_HANDLER(3,11);
BSP_ISR_DEFINE_HANDLER(3,12);
BSP_ISR_DEFINE_HANDLER(3,13);
BSP_ISR_DEFINE_HANDLER(3,14);
BSP_ISR_DEFINE_HANDLER(3,15);
BSP_ISR_DEFINE_HANDLER(3,16);
BSP_ISR_DEFINE_HANDLER(3,17);
BSP_ISR_DEFINE_HANDLER(3,18);
BSP_ISR_DEFINE_HANDLER(3,19);

BSP_ISR_DEFINE_HANDLER(3,20);
BSP_ISR_DEFINE_HANDLER(3,21);
BSP_ISR_DEFINE_HANDLER(3,22);
BSP_ISR_DEFINE_HANDLER(3,23);
BSP_ISR_DEFINE_HANDLER(3,24);
BSP_ISR_DEFINE_HANDLER(3,25);
BSP_ISR_DEFINE_HANDLER(3,26);
BSP_ISR_DEFINE_HANDLER(3,27);
BSP_ISR_DEFINE_HANDLER(3,28);
BSP_ISR_DEFINE_HANDLER(3,29);

BSP_ISR_DEFINE_HANDLER(3,30);
BSP_ISR_DEFINE_HANDLER(3,31);

/* ------------------------------------------------------------------------------------------------
 * CORE 4
 * ----------------------------------------------------------------------------------------------*/
#if (UC_NB_CORES > 4)

BSP_ISR_VECTOR_TABLE(4);

BSP_ISR_DEFINE_HANDLER(4,0);
BSP_ISR_DEFINE_HANDLER(4,1);
BSP_ISR_DEFINE_HANDLER(4,2);
BSP_ISR_DEFINE_HANDLER(4,3);
BSP_ISR_DEFINE_HANDLER(4,4);
BSP_ISR_DEFINE_HANDLER(4,5);
BSP_ISR_DEFINE_HANDLER(4,6);
BSP_ISR_DEFINE_HANDLER(4,7);
BSP_ISR_DEFINE_HANDLER(4,8);
BSP_ISR_DEFINE_HANDLER(4,9);

BSP_ISR_DEFINE_HANDLER(4,10);
BSP_ISR_DEFINE_HANDLER(4,11);
BSP_ISR_DEFINE_HANDLER(4,12);
BSP_ISR_DEFINE_HANDLER(4,13);
BSP_ISR_DEFINE_HANDLER(4,14);
BSP_ISR_DEFINE_HANDLER(4,15);
BSP_ISR_DEFINE_HANDLER(4,16);
BSP_ISR_DEFINE_HANDLER(4,17);
BSP_ISR_DEFINE_HANDLER(4,18);
BSP_ISR_DEFINE_HANDLER(4,19);

BSP_ISR_DEFINE_HANDLER(4,20);
BSP_ISR_DEFINE_HANDLER(4,21);
BSP_ISR_DEFINE_HANDLER(4,22);
BSP_ISR_DEFINE_HANDLER(4,23);
BSP_ISR_DEFINE_HANDLER(4,24);
BSP_ISR_DEFINE_HANDLER(4,25);
BSP_ISR_DEFINE_HANDLER(4,26);
BSP_ISR_DEFINE_HANDLER(4,27);
BSP_ISR_DEFINE_HANDLER(4,28);
BSP_ISR_DEFINE_HANDLER(4,29);

BSP_ISR_DEFINE_HANDLER(4,30);
BSP_ISR_DEFINE_HANDLER(4,31);

/* ------------------------------------------------------------------------------------------------
 * CORE 5
 * ----------------------------------------------------------------------------------------------*/
#if (UC_NB_CORES > 5)

BSP_ISR_VECTOR_TABLE(5);

BSP_ISR_DEFINE_HANDLER(5,0);
BSP_ISR_DEFINE_HANDLER(5,1);
BSP_ISR_DEFINE_HANDLER(5,2);
BSP_ISR_DEFINE_HANDLER(5,3);
BSP_ISR_DEFINE_HANDLER(5,4);
BSP_ISR_DEFINE_HANDLER(5,5);
BSP_ISR_DEFINE_HANDLER(5,6);
BSP_ISR_DEFINE_HANDLER(5,7);
BSP_ISR_DEFINE_HANDLER(5,8);
BSP_ISR_DEFINE_HANDLER(5,9);

BSP_ISR_DEFINE_HANDLER(5,10);
BSP_ISR_DEFINE_HANDLER(5,11);
BSP_ISR_DEFINE_HANDLER(5,12);
BSP_ISR_DEFINE_HANDLER(5,13);
BSP_ISR_DEFINE_HANDLER(5,14);
BSP_ISR_DEFINE_HANDLER(5,15);
BSP_ISR_DEFINE_HANDLER(5,16);
BSP_ISR_DEFINE_HANDLER(5,17);
BSP_ISR_DEFINE_HANDLER(5,18);
BSP_ISR_DEFINE_HANDLER(5,19);

BSP_ISR_DEFINE_HANDLER(5,20);
BSP_ISR_DEFINE_HANDLER(5,21);
BSP_ISR_DEFINE_HANDLER(5,22);
BSP_ISR_DEFINE_HANDLER(5,23);
BSP_ISR_DEFINE_HANDLER(5,24);
BSP_ISR_DEFINE_HANDLER(5,25);
BSP_ISR_DEFINE_HANDLER(5,26);
BSP_ISR_DEFINE_HANDLER(5,27);
BSP_ISR_DEFINE_HANDLER(5,28);
BSP_ISR_DEFINE_HANDLER(5,29);

BSP_ISR_DEFINE_HANDLER(5,30);
BSP_ISR_DEFINE_HANDLER(5,31);

#endif // CORE 5
#endif // CORE 4
#endif // CORE 3
#endif // CORE 2
#endif // CORE 1
#endif // CORE 0


#endif /* #ifndef BSP_ISR_SUPPORT_DISABLED */
