#ifndef __SHARED_MEMORY__
#define __SHARED_MEMORY__

#include <stdint.h>
#define NS_SH(name) SMEM_##name
#define NS_SD(name) NS_SH(DATA_##name)

#define AMK_SHARED_VARS(engine)\
  NS_SD(AMK_STATUS_##engine),\
  NS_SD(AMK_ACTUAL_VELOCITY_##engine),\
  NS_SD(AMK_TORQUE_CURRENT_##engine),\
  NS_SD(AMK_VOLTAGE_##engine),\
  NS_SD(AMK_CURRENT_##engine), /*INFO: to remove. Exist only for legacy compatibility*/ \
  NS_SD(AMK_TEMP_MOTOR_##engine),\
  NS_SD(AMK_TEMP_INVERTER_##engine),\
  NS_SD(AMK_TEMP_IGBT_##engine),\
  NS_SD(AMK_ERROR_INFO_##engine),\
  NS_SD(AMK_LIMIT_POSITIVE_##engine),\
  NS_SD(AMK_LIMIT_NEGATIVE_##engine),\

typedef uint32_t SharedDataType;

enum NS_SD()
{
  NS_SD(GO_STATUS)=0,
  NS_SD(START_RES),
  NS_SD(BRE_IMP),
  AMK_SHARED_VARS(FL)
  AMK_SHARED_VARS(FR)
  AMK_SHARED_VARS(RL)
  AMK_SHARED_VARS(RR)

  NS_SD(__NUM_OF__)
};

void NS_SH(update)(const enum NS_SD() param, const SharedDataType value);
SharedDataType NS_SH(get)(const enum NS_SD() param);

#endif // !__SHARED_MEMORY__
