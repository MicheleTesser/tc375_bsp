/**
 * \file ConfigurationIsr.h
 * \brief Interrupts configuration.
 *
 *
 * \version disabled
 * \copyright Copyright (c) 2014 Infineon Technologies AG. All rights reserved.
 *
 *
 *                                 IMPORTANT NOTICE
 *
 *
 * Infineon Technologies AG (Infineon) is supplying this file for use
 * exclusively with Infineon's microcontroller products. This file can be freely
 * distributed within development tools that are supporting such microcontroller
 * products.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * INFINEON SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 * \defgroup IfxLld_Demo_StmDemo_SrcDoc_InterruptConfig Interrupt configuration
 * \ingroup IfxLld_Demo_StmDemo_SrcDoc
 */

#ifndef CONFIGURATIONISR_H
#define CONFIGURATIONISR_H
/******************************************************************************/
/*-----------------------------------Macros-----------------------------------*/
/******************************************************************************/

/** \brief Build the ISR configuration object
 * \param no interrupt priority
 * \param cpu assign CPU number
 */
#define ISR_ASSIGN(no, cpu)  ((no << 8) + cpu)

/** \brief extract the priority out of the ISR object */
#define ISR_PRIORITY(no_cpu) (no_cpu >> 8)

/** \brief extract the service provider  out of the ISR object */
#define ISR_PROVIDER(no_cpu) (no_cpu % 8)

//------------------------------------------------------------------------------

#endif
