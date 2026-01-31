/******************************************************************************
 * FILE:
 *    illd_uc.h
 *
 * UC ARCHITECTURE:
 *    TC3x
 *
 * UC FAMILY:
 *    TC39x
 *
 * DESCRIPTION:
 *    Low Level Driver API interface for AURIX TC29x uC family
 *
 * FILE HISTORY:
 *    V1    06.2020    RO
 *        > Initial version
 *
 ******************************************************************************
 * Copyright (C) 2015-2020 HighTec EDV-Systeme GmbH.  All rights reserved.
 * This is proprietary software. Any use of the software requires a written
 * license agreement with HighTec EDV-Systeme GmbH. Please contact for
 * detailed license information: info@hightec-rt.com.
 *****************************************************************************/

#ifndef ILLD_UC_H
#define ILLD_UC_H

/* ============================================================================
 * Include paths
 * ==========================================================================*/

/* ============================================================================
 * uC LLD API
 * ==========================================================================*/

/* STM (timer) */
extern void     illd_uc_stm_InitChannel(uint32_t channel, uint32_t coreId, uint32_t prio, uint32_t reload_value);
extern void     illd_board_led_InitAll(BOARD_LED_SET_e set);
extern void     illd_board_led_Set(BOARD_LED_e led, BOARD_LED_SET_e set);
extern void     illd_uc_core_StartAllCores(void);
extern void     illd_uc_core_EnableICache(void);
extern void     illd_uc_intc_stm_SetSRC(uint32_t channel, uint32_t coreId, uint32_t prio);
extern void     illd_uc_stm_ClearChannelIsrFlag(uint32_t channel);
extern void     illd_uc_stm_ReloadChannel(uint32_t channel, uint32_t reload_value);
extern uint32_t illd_uc_core_GetCurrentCore(void);
extern void     illd_IfxCpu_enableInterrupts(void);
extern void     illd_uc_scu_wdt_DisableCpuWatchdog(void);
extern void     illd_uc_scu_wdt_DisableSafetyWatchdog(void);
extern void     illd_uc_InitClock(void);
//extern Ifx_P *uc_get_led_port(uint32_t board_led_port)

#endif /* ILLD_UC_H */
