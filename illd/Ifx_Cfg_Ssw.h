#ifndef IFX_CFG_SSW_H_
#define IFX_CFG_SSW_H_

#include "Ifx_Cfg.h"
#include "Ifx_Ssw.h"

/* Minimal startup configuration for the example. */
#ifndef IFX_CFG_SSW_ENABLE_PMS_INIT
#define IFX_CFG_SSW_ENABLE_PMS_INIT       (0U)
#endif
#ifndef IFX_CFG_SSW_ENABLE_PMS_INIT_CHECK
#define IFX_CFG_SSW_ENABLE_PMS_INIT_CHECK (0U)
#endif
#ifndef IFX_CFG_SSW_ENABLE_LBIST
#define IFX_CFG_SSW_ENABLE_LBIST          (0U)
#endif
#ifndef IFX_CFG_SSW_ENABLE_MONBIST
#define IFX_CFG_SSW_ENABLE_MONBIST        (0U)
#endif
#ifndef IFX_CFG_SSW_ENABLE_MMIC_CHECK
#define IFX_CFG_SSW_ENABLE_MMIC_CHECK     (0U)
#endif
#ifndef IFX_CFG_SSW_ENABLE_PLL_INIT
#define IFX_CFG_SSW_ENABLE_PLL_INIT       (1U)
#endif
#ifndef IFX_CFG_SSW_ENABLE_MBIST
#define IFX_CFG_SSW_ENABLE_MBIST          (0U)
#endif
#ifndef IFX_CFG_SSW_ENABLE_SMU
#define IFX_CFG_SSW_ENABLE_SMU            (0U)
#endif
#ifndef IFX_CFG_SSW_ENABLE_EMEM_INIT
#define IFX_CFG_SSW_ENABLE_EMEM_INIT      (0U)
#endif

#if IFX_CFG_SSW_ENABLE_PLL_INIT == 1U
#include "IfxScuCcu.h"
#define IFX_CFG_SSW_CALLOUT_PLL_INIT()                          \
    {                                                           \
        if (IfxScuCcu_init(&IfxScuCcu_defaultClockConfig) == 1) \
        {                                                       \
            __debug();                                          \
        }                                                       \
    }
#endif

#endif /* IFX_CFG_SSW_H_ */
