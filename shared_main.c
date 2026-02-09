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

#include "Configuration.h"
#include "IfxCpu.h"
#include "IfxCpu_Intrinsics.h"
#include "IfxCpu_reg.h"
#include "IfxScuCcu.h"
#include "IfxScuWdt.h"
#include "cores/cores.h"
#include <stdint.h>

extern unsigned int __INTTAB_CPU0[];
extern unsigned int __INTTAB_CPU1[];
extern unsigned int __INTTAB_CPU2[];

IFX_STATIC IFX_CONST IfxScuCcu_FlashWaitstateConfig IfxScuCcu_defaultFlashWaitstateConfig = IFXSCU_CFG_FLASH_WAITSTATE;

IFX_STATIC IFX_CONST IfxScuCcu_PllStepConfig IfxScuCcu_defaultPllConfigSteps[] = {
    IFXSCU_CFG_PLL_STEPS
};

IFX_CONST IfxScuCcu_Config clockConfig = {
    IFXSCU_CFG_PLL_INITIAL_STEP,
    {
        (uint8)(sizeof(IfxScuCcu_defaultPllConfigSteps) / sizeof(IfxScuCcu_PllStepConfig)),
        IfxScuCcu_defaultPllConfigSteps
    },
    IFXSCU_CFG_CLK_DISTRIBUTION,
    &IfxScuCcu_defaultFlashWaitstateConfig,
    &IfxScuCcu_defaultModConfig
};

extern void _crt0_reset(void);

static void startAllCores(void)
{
  uint32_t currentCore = IfxCpu_getCoreIndex();

#if (IFXCPU_NUM_MODULES > 1)
  if (currentCore != 1U) {
    IfxCpu_startCore(&MODULE_CPU1, (uint32_t)&_crt0_reset);
  }
#endif
#if (IFXCPU_NUM_MODULES > 2)
  if (currentCore != 2U) {
    IfxCpu_startCore(&MODULE_CPU2, (uint32_t)&_crt0_reset);
  }
#endif
}

static void setBiv(uint32_t table)
{
  uint16_t pw = IfxScuWdt_getCpuWatchdogPassword();
  uint32_t biv = ((uint32_t)0xFFFFFFFEU & table);

  IfxScuWdt_clearCpuEndinit(pw);
  __mtcr(CPU_BIV, biv);
  IfxScuWdt_setCpuEndinit(pw);
  __isync();
}

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
  uint32_t coreId = IfxCpu_getCoreIndex();

  IfxCpu_enableInterrupts();

  switch (coreId) {
  case 0U:
    main_0();
    break;
#if (IFXCPU_NUM_MODULES > 1)
  case 1U:
    main_1();
    break;
#endif
#if (IFXCPU_NUM_MODULES > 2)
  case 2U:
    main_2();
    break;
#endif
  default:
    break;
  }

  for (;;) {
  }
}

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: Crt0PreInit()
 *     Crt0 Hook before memory clear & copy operation take place.
 *     The code cannot rely on initialized and cleared global data !
 * ------------------------------------------------------------------------------------------------
 */
void Crt0PreInit(void) {
  uint32_t coreId = IfxCpu_getCoreIndex();

  if (coreId == 0U) {
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());
  }

  IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());

  IfxCpu_invalidateProgramCache();
  IfxCpu_setDataCache(0);
  IfxCpu_setProgramCache(1);
}

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: Crt0PostInit()
 *     Crt0 Hook after complete crt0 initialization, just before shared_main()
 * entry point is called. It is the place of uC shared HW resource
 * initialization.
 * ------------------------------------------------------------------------------------------------
 */
void Crt0PostInit(void) {
  uint32_t coreId = IfxCpu_getCoreIndex();

  if (coreId == 0U) {
    IfxScuCcu_init(&clockConfig);
    startAllCores();
  }

  switch (coreId) {
  case 0U:
    setBiv((uint32_t)__INTTAB_CPU0);
    break;
#if (IFXCPU_NUM_MODULES > 1)
  case 1U:
    setBiv((uint32_t)__INTTAB_CPU1);
    break;
#endif
#if (IFXCPU_NUM_MODULES > 2)
  case 2U:
    setBiv((uint32_t)__INTTAB_CPU2);
    break;
#endif
  default:
    break;
  }
}
