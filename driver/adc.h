#ifndef __VIRTUAL_ADC__
#define __VIRTUAL_ADC__

#include <stdint.h>

#define NS_ADC(name) ADC_##name

typedef struct __attribute__((aligned(4))) {
  const uint8_t private_data[4];
} Adc_h;

typedef uint32_t AdcValue;

#define NS_ADP(name) NS_ADC(PIN_##name)
enum NS_ADP() {
  NS_ADP(BRAKE_PRESSURE_FRONT) = 0,
  NS_ADP(BRAKE_PRESSURE_REAR),

  NS_ADP(__NUM_OF__)
};

extern int8_t NS_ADC(init)(Adc_h *const restrict self, const enum NS_ADP() pin)
    __attribute__((__nonnull__(1)));

extern AdcValue NS_ADC(read)(Adc_h *const restrict self)
    __attribute__((__nonnull__(1)));

extern void NS_ADC(destroy)(Adc_h *const restrict self)
    __attribute__((__nonnull__(1)));

#endif // !__VIRTUAL_ADC__
