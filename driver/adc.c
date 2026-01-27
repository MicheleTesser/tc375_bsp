#include "adc.h"
#include "bsp.h"

#include "uc/uc_tc37/TC37xPD_RegCHeaders/_Reg/IfxEvadc_reg.h"

#include <stdatomic.h>
#include <stdint.h>
#include <string.h>

struct Adc_t {
  uint32_t pin;
};

union Adc_h_t_conv {
  Adc_h *const restrict hidden;
  struct Adc_t *const restrict clear;
};

#ifdef DEBUG_BUILD
char __assert_adc_size[sizeof(Adc_h) == sizeof(struct Adc_t) ? 1 : -1];
char __assert_adc_align[_Alignof(Adc_h) == _Alignof(struct Adc_t) ? 1 : -1];
#endif /* ifdef DEBUG_BUILD */

typedef struct {
  uint8_t group_id;
  uint8_t channel_id;
  atomic_flag taken;
} AdcInfo;

static AdcInfo g_adc_info[NS_ADP(__NUM_OF__)] = {
    {.group_id = 0U, .channel_id = 3U},
    {.group_id = 0U, .channel_id = 1U},
};

static atomic_flag g_evadc_init;
static atomic_flag g_group0_init;
static atomic_flag g_queue0_started;

static void evadc_init_module(void) {
  Ifx_EVADC *evadc = &MODULE_EVADC;
  if (atomic_flag_test_and_set(&g_evadc_init)) {
    return;
  }

  evadc->CLC.B.DISR = 0U;
  while (evadc->CLC.B.DISS != 0U) {
  }

  evadc->GLOBCFG.B.SUCAL = 0U;
}

static void evadc_init_group0(void) {
  Ifx_EVADC *evadc = &MODULE_EVADC;
  Ifx_EVADC_G *g0 = &evadc->G[0];

  if (atomic_flag_test_and_set(&g_group0_init)) {
    return;
  }

  g0->ANCFG.B.BE = 1U;
  g0->ANCFG.B.IPE = 0U;
  g0->ANCFG.B.RPE = 0U;

  g0->ICLASS[0].B.STCS = 3U;
  g0->ICLASS[0].B.CMS = 0U;
  g0->ICLASS[0].B.AIPS = 0U;

  g0->ARBPR.B.ASEN0 = 1U;
  g0->ARBPR.B.PRIO0 = 3U;
  g0->ARBPR.B.CSM0 = 0U;

  g0->ARBCFG.B.ANONC = 1U;

  g0->Q[0].QMR.B.ENGT = 3U;
  g0->Q[0].QMR.B.RPTDIS = 0U;
  g0->Q[0].QMR.B.FLUSH = 1U;
  g0->Q[0].QMR.B.CEV = 1U;
}

int8_t NS_ADC(init)(Adc_h *const restrict self, const enum NS_ADP() pin) {
  union Adc_h_t_conv conv = {self};
  struct Adc_t *const p_self = conv.clear;
  Ifx_EVADC *evadc = &MODULE_EVADC;
  Ifx_EVADC_G *g0 = &evadc->G[0];
  uint8_t channel;

  if (pin >= NS_ADP(__NUM_OF__)) {
    return -1;
  }
  if (atomic_flag_test_and_set(&g_adc_info[pin].taken)) {
    return -1;
  }

  evadc_init_module();
  evadc_init_group0();

  channel = g_adc_info[pin].channel_id;
  g0->CHCTR[channel].B.ICLSEL = 0U;
  g0->CHCTR[channel].B.RESREG = channel;
  g0->CHCTR[channel].B.RESPOS = 0U;
  g0->CHCTR[channel].B.RESTGT = 0U;

  g0->Q[0].QINR.U = (uint32_t)channel | (1U << 5);

  if (!atomic_flag_test_and_set(&g_queue0_started)) {
    g0->Q[0].QMR.B.ENGT = 3U;
    g0->Q[0].QMR.B.TREV = 1U;
  }

  p_self->pin = (uint32_t)pin;
  return 0;
}

AdcValue NS_ADC(read)(Adc_h *const restrict self) {
  union Adc_h_t_conv conv = {self};
  struct Adc_t *const p_self = conv.clear;
  Ifx_EVADC *evadc = &MODULE_EVADC;
  Ifx_EVADC_G *g0 = &evadc->G[0];
  uint8_t channel = g_adc_info[(enum NS_ADP())p_self->pin].channel_id;
  uint32_t tries = 0U;
  Ifx_EVADC_G_RES res;

  res.U = 0U;
  while ((res.B.VF == 0U) && (tries < 32U)) {
    res = g0->RES[channel];
    tries++;
  }

  if (res.B.VF == 0U) {
    return 0U;
  }

  return (AdcValue)res.B.RESULT;
}

void NS_ADC(destroy)(Adc_h *const restrict self) {
  union Adc_h_t_conv conv = {self};
  struct Adc_t *const p_self = conv.clear;
  atomic_flag_clear(&g_adc_info[(enum NS_ADP())p_self->pin].taken);
  memset(p_self, 0, sizeof(*p_self));
}
