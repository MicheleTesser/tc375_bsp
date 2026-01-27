#include "./compontent.h"
#include "bsp.h"

#include <stddef.h>
#include <stdint.h>
#include <string.h>

// gpio

struct GpioRead_t {
  uint32_t pin_gpio;
};

static struct GpioSpec {
  uint32_t port;
  uint8_t pin_index;
  uint8_t is_output;
} BOARD_GPIOS[__NUM_OF_GPIOS__] = {
    {33, 0, 1}, // INFO: GPIO_CORE_0_ALIVE_BLINK, default led 0
    {33, 4, 1}, // INFO: GPIO_CORE_1_ALIVE_BLINK, debug led 2 1
    {33, 6, 1}, // INFO: GPIO_CORE_2_ALIVE_BLINK, debug led 3 2
    {23, 1, 0}, // INFO: GPIO_RTD_BUTTON, DRIVE_BUTTON 3
    {33, 5, 1}, // INFO: GPIO_RTD_ASSI_SOUND, RTDS 4
    {32, 4, 0}, // INFO: GPIO_AIR_PRECHARGE_INIT, AIR_1 5
    {23, 0, 0}, // INFO: GPIO_AIR_PRECHARGE_DONE, AIR_2 6
    {33, 3, 1}, // INFO: GPIO_SCS, SCS_SDC_RELAY 7
    {33, 2, 1}, // INFO: GPIO_AS_NODE, AS_SDC_RELAY 8
    {33, 1, 1}, // INFO: GPIO_RTD_BUTTON_LED, RTD 9
    {21, 0, 1}, // INFO: GPIO_ASSI_LIGHT_BLUE, 10
    {15, 4, 1}, // INFO: GPIO_ASSI_LIGHT_YELLOW, 11
    {23, 4, 1}, // INFO: GPIO_AMS, 12
    {23, 5, 1}, // INFO: GPIO_IMD, 13
};

union GpioRead_h_t_conv {
  GpioRead_h *const restrict hidden;
  struct GpioRead_t *const restrict clear;
};

union GpioRead_h_t_conv_const {
  const GpioRead_h *const restrict hidden;
  const struct GpioRead_t *const restrict clear;
};

struct Gpio_t {
  struct GpioRead_t gpio_read_permission;
  uint8_t private_data[4];
};

union Gpio_h_t_conv {
  Gpio_h *const restrict hidden;
  struct Gpio_t *const restrict clear;
};
#ifdef DEBUG_BUILD
char __assert_gpio_size[sizeof(GpioRead_h) == sizeof(struct GpioRead_t) ? +1
                                                                        : -1];
char __assert_gpio_align[_Alignof(GpioRead_h) == _Alignof(struct GpioRead_t)
                             ? +1
                             : -1];

char __assert_gpio_size[sizeof(Gpio_h) == sizeof(struct Gpio_t) ? +1 : -1];
char __assert_gpio_align[_Alignof(Gpio_h) == _Alignof(struct Gpio_t) ? +1 : -1];

#endif /* ifdef DEBUG_BUILD */

// public

int8_t hardware_init_gpio(Gpio_h *const restrict self, const enum GPIO_PIN id,
                          const uint8_t default_v) {
  union Gpio_h_t_conv conv = {self};
  struct Gpio_t *p_self = conv.clear;
  struct GpioSpec *p_info_gpio = NULL;

  if (id >= __NUM_OF_GPIOS__) {
    return -1;
  }

  p_info_gpio = &BOARD_GPIOS[id];

  p_self->gpio_read_permission.pin_gpio = id;

  if (p_info_gpio->is_output) {
    if (!default_v) {
      bsp_uc_port_SetGPIO(p_info_gpio->port, p_info_gpio->pin_index,
                          UC_PORT_OUTPUT_SET);
    } else {
      bsp_uc_port_SetGPIO(p_info_gpio->port, p_info_gpio->pin_index,
                          UC_PORT_OUTPUT_CLEAR);
    }
    bsp_uc_port_EnableOutput(p_info_gpio->port, p_info_gpio->pin_index, 0,
                             UC_PORT_PL_LEVEL_AL, UC_PORT_PD_SPEED_3);
  } else {
    bsp_uc_port_EnableInput(p_info_gpio->port, p_info_gpio->pin_index,
                            UC_PORT_INPUT_MODE_TRISTATE);
  }

  return 0;
}

int8_t hardware_init_read_permission_gpio(GpioRead_h *const restrict self,
                                          const uint16_t id) {
  union GpioRead_h_t_conv conv = {self};
  struct GpioRead_t *p_self = conv.clear;
  struct GpioSpec *p_info_gpio = NULL;
  p_info_gpio = &BOARD_GPIOS[id];

  p_self->pin_gpio = id;
  if (p_info_gpio->is_output) {
    bsp_uc_port_EnableOutput(p_info_gpio->port, p_info_gpio->pin_index, 0,
                             UC_PORT_PL_LEVEL_AL, UC_PORT_PD_SPEED_3);
  } else {
    bsp_uc_port_EnableInput(p_info_gpio->port, p_info_gpio->pin_index,
                            UC_PORT_INPUT_MODE_TRISTATE);
  }

  return 0;
}

int8_t gpio_toggle(Gpio_h *const restrict self) {
  union Gpio_h_t_conv conv = {self};
  struct Gpio_t *p_self = conv.clear;
  struct GpioSpec *p_info_gpio =
      &BOARD_GPIOS[p_self->gpio_read_permission.pin_gpio];

  bsp_uc_port_SetGPIO(p_info_gpio->port, p_info_gpio->pin_index,
                      UC_PORT_OUTPUT_TOGGLE);
  return 0;
}
int8_t gpio_read_state(const GpioRead_h *const restrict self) {
  const union GpioRead_h_t_conv_const conv = {self};
  const struct GpioRead_t *p_self = conv.clear;
  struct GpioSpec *p_info_gpio = &BOARD_GPIOS[p_self->pin_gpio];

  return (int8_t)bsp_uc_port_GetGPIO(p_info_gpio->port, p_info_gpio->pin_index);
}

int8_t gpio_set_high(Gpio_h *const restrict self) {
  union Gpio_h_t_conv conv = {self};
  struct Gpio_t *p_self = conv.clear;
  struct GpioSpec *p_info_gpio =
      &BOARD_GPIOS[p_self->gpio_read_permission.pin_gpio];

  bsp_uc_port_SetGPIO(p_info_gpio->port, p_info_gpio->pin_index,
                      UC_PORT_OUTPUT_SET);
  return 0;
}

int8_t gpio_set_low(Gpio_h *const restrict self) {
  union Gpio_h_t_conv conv = {self};
  struct Gpio_t *p_self = conv.clear;
  struct GpioSpec *p_info_gpio =
      &BOARD_GPIOS[p_self->gpio_read_permission.pin_gpio];

  bsp_uc_port_SetGPIO(p_info_gpio->port, p_info_gpio->pin_index,
                      UC_PORT_OUTPUT_CLEAR);

  return 0;
}

int8_t gpio_destroy(Gpio_h *const restrict self) {
  memset(self, 0, sizeof(*self));
  return 0;
}

int8_t gpio_read_destroy(GpioRead_h *const restrict self) {
  memset(self, 0, sizeof(*self));
  return 0;
}
