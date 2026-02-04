#include "component.h"
#include "IfxEvadc_Adc.h"
#include <IfxEvadc_Adc.h>

#include <sys/cdefs.h>
#include <stdatomic.h>
#include <stdint.h>
#include <string.h>


struct Adc_t
{
  enum NS_ADP() pin;
};

union Adc_h_t_conv
{
  Adc_h* const restrict hidden;
  struct Adc_t* const restrict clear;
};

#ifdef DEBUG
char __assert_size_adc[sizeof(Adc_h)==sizeof(struct Adc_t)?1:-1];
char __assert_align_adc[_Alignof(Adc_h)==_Alignof(struct Adc_t)?1:-1];
#endif /* ifdef DEBUG */

static struct{
  IfxEvadc_GroupId group_id;
  IfxEvadc_ChannelId channel_id;
  IfxEvadc_Adc_Channel g_adcChannel;
  atomic_flag taken;
}ADC_INFO[NS_ADP(__NUM_OF__)] =
{
  {.group_id = IfxEvadc_GroupId_0, .channel_id = (IfxEvadc_ChannelId) 3, .taken = ATOMIC_FLAG_INIT}, //INFO: NS_ADP(BRAKE_PRESSURE_FRONT) (0)
  {.group_id = IfxEvadc_GroupId_0, .channel_id = (IfxEvadc_ChannelId) 1, .taken = ATOMIC_FLAG_INIT}, //INFO: NS_ADP(BRAKE_PRESSURE_REAR) (1)
};
static struct {
  IfxEvadc_Adc G_EVADC;
  IfxEvadc_Adc_Group g_adcGroup;
  atomic_flag init;
}G_EVADC_STATUS = {
  .init = ATOMIC_FLAG_INIT
};

static atomic_flag group0_queue_started;
static atomic_flag group0_group_started;

//public

int8_t NS_ADC(init)(Adc_h* const restrict self, const enum NS_ADP() pin)
{
  union Adc_h_t_conv conv = {self};
  struct Adc_t* const p_self = conv.clear;
  IfxEvadc_Adc_GroupConfig adcGroupConfig;
  IfxEvadc_Adc_ChannelConfig adcChannelConfig;

  if (pin >= NS_ADP(__NUM_OF__) || atomic_flag_test_and_set(&ADC_INFO[pin].taken))
  {
    return -1;
  }

  if(!atomic_flag_test_and_set(&G_EVADC_STATUS.init))
  {
   IfxEvadc_Adc_Config adcConfig;
   IfxEvadc_Adc_initModuleConfig(&adcConfig, &MODULE_EVADC);

   /* Initialize module */
   IfxEvadc_Adc_initModule(&G_EVADC_STATUS.G_EVADC, &adcConfig);
  }

  IfxEvadc_Adc_initGroupConfig(&adcGroupConfig, &G_EVADC_STATUS.G_EVADC);

  /* Setting user configuration using group 0 */
  adcGroupConfig.groupId = ADC_INFO[pin].group_id;
  adcGroupConfig.master = ADC_INFO[pin].group_id;
  // adcGroupConfig.startupCalibration = TRUE;

  /* enable all arbiter request sources */
  adcGroupConfig.arbiter.requestSlotQueue0Enabled = TRUE; // enable Queue0 mode
  // adcGroupConfig.arbiter.requestSlotQueue1Enabled = TRUE; // enable Queue1 mode
  // adcGroupConfig.arbiter.requestSlotQueue2Enabled = TRUE; // enable Queue2 mode

  // enable all gates in "always" mode (no edge detection)
  adcGroupConfig.queueRequest[0].triggerConfig.gatingMode = IfxEvadc_GatingMode_always;
  // adcGroupConfig.queueRequest[1].triggerConfig.gatingMode = IfxEvadc_GatingMode_always;
  // adcGroupConfig.queueRequest[2].triggerConfig.gatingMode = IfxEvadc_GatingMode_always;

  /* Initialize the group */
  if(!atomic_flag_test_and_set(&group0_group_started)){
    IfxEvadc_Adc_initGroup(&G_EVADC_STATUS.g_adcGroup, &adcGroupConfig);
  }
  // IfxEvadc_RequestSource requestSource = IfxEvadc_RequestSource_queue0;
 
  // IfxEvadc_GatingMode savedGate __attribute__((__unused__)) =  IfxEvadc_getQueueSlotGatingMode(ADC_INFO[pin].g_adcGroup.group,requestSource);
  // IfxEvadc_GatingSource gatingSource = IfxEvadc_getQueueSlotGatingSource(ADC_INFO[pin].g_adcGroup.group, requestSource );

  // IfxEvadc_setQueueSlotGatingConfig(ADC_INFO[pin].g_adcGroup.group, gatingSource, IfxEvadc_GatingMode_disabled, requestSource );
  /* Initialize the configuration with default values */
  IfxEvadc_Adc_initChannelConfig(&adcChannelConfig, &G_EVADC_STATUS.g_adcGroup);

  /* Select the channel ID and the respective result register */
  adcChannelConfig.channelId = (IfxEvadc_ChannelId)ADC_INFO[pin].channel_id;

  adcChannelConfig.resultRegister = (IfxEvadc_ChannelResult)ADC_INFO[pin].channel_id;

  IfxEvadc_Adc_initChannel(&ADC_INFO[pin].g_adcChannel, &adcChannelConfig);
  IfxEvadc_Adc_addToQueue(&ADC_INFO[pin].g_adcChannel, IfxEvadc_RequestSource_queue0, IFXEVADC_QUEUE_REFILL);

  ///* Start the queue */
  if (!atomic_flag_test_and_set(&group0_queue_started)) {
    IfxEvadc_Adc_startQueue(&G_EVADC_STATUS.g_adcGroup, IfxEvadc_RequestSource_queue0);
  }

  p_self->pin = pin;
  return 0;
}

AdcValue NS_ADC(read)(Adc_h* const restrict self)
{
  union Adc_h_t_conv conv = {self};
  struct Adc_t* const p_self = conv.clear;
  uint32_t tries =0;

  Ifx_EVADC_G_RES conversionResult;                                   /* Variable to store the conversion result  */
  conversionResult.U = 0;                                             /* Initialize the value to 0                */


  while((conversionResult.B.VF != 1 || conversionResult.B.DRC != 0) && tries < 32)
  {
    conversionResult = IfxEvadc_Adc_getResult(&ADC_INFO[p_self->pin].g_adcChannel);
    tries++;
  }

  if (tries >= 32)
  {
    return 0;
  }
  return conversionResult.B.RESULT;
}

void NS_ADC(destroy)(Adc_h* const restrict self)
{
  union Adc_h_t_conv conv = {self};
  struct Adc_t* const p_self = conv.clear;

  atomic_flag_clear(&ADC_INFO[p_self->pin].taken);
  memset(p_self, 0, sizeof(*p_self));
}

