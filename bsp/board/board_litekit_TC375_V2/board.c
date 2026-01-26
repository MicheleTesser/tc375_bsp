/**************************************************************************************************
 * FILE: board_appkit_tc3x7.c
 *
 * BOARD FAMILY:
 *
 *    LiteKit TC375 V2
 *
 * UC SUPPORT:
 *
 *    TC375     (LQFP-176 package)
 *
 * DESCRIPTION:
 *
 *    Implementation of Board support routines.
 *
 * HISTORY:
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

/* board implementation guard */
#if defined (LITEKIT_TC375_V2)

/* include common BSP header file */
#include "bsp.h"


/* ------------------------------------------------------------------------------------------------
 * Board LED port-pin setup
 *    The EVB has only 2 on-board LEDs, the rest is GPIOs
 * ----------------------------------------------------------------------------------------------*/

const BOARD_LED_S board_led[BOARD_NB_LEDS] =
{
	{0, 5},  /* LED_0 */
	{0, 6},  /* LED_1 */
};


#endif /* #if defined (LITEKIT_TC375_V2) */
