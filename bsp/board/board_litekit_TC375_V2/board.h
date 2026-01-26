/**************************************************************************************************
 * FILE: board.h
 *
 * BOARD FAMILY:
 *
 *    LiteKit TC375 V2
 *
 * UC SUPPORT:
 *
 *    TC375     (LQFP-176 package)
 
 * DESCRIPTION:
 *
 *    Board support package specification and types.
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

#ifndef BOARD_H
#define BOARD_H


/* BSP Board low level specification header files */
#include "board_spec.h"
#include "board_types.h"

/* Board configuration */
extern const BOARD_LED_S board_led[BOARD_NB_LEDS];


#endif /* BOARD_H */
#endif /* LITEKIT_TC375_V2 */
