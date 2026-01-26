/**************************************************************************************************
 * FILE: bsp_isr.h
 *
 * UC ARCHITECTURE:
 *
 *    AURIX TC3x
 *
 * UC FAMILY:
 *
 *    TC39x
 *
 * DESCRIPTION:
 *
 *    BSP Interrupt interface file
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

#ifndef BSP_ISR_H_
#define BSP_ISR_H_


/* ================================================================================================
 * DEFINES
 * ==============================================================================================*/

/* Number of BSP supported Interrupt Vectors
 * Must correspond to number of vectors in bsp_isr_vector_table.c file
 * that are created manually
 */
#define BSP_ISR_MAX_VECTORS            32


/* ================================================================================================
 * TYPEDEFS
 * ==============================================================================================*/

/* BSP ISR Handler Prototype */
typedef void (*BspIsrHandler)(void);


/* ================================================================================================
 * EXTERN DECLARATION
 * ==============================================================================================*/

/* Default Interrupt Handler for Undefined Interrupt Handlers (not mapped to any HW module) */
extern void isr_undef_handler(void);


#endif /* BSP_ISR_H_ */
