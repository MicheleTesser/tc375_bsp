#include "serial.h"
#include "bsp.h"

#include "uc/uc_tc37/TC37xPD_RegCHeaders/_Reg/IfxAsclin_reg.h"

#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define SERIAL_ASCLIN (&MODULE_ASCLIN0)
#define SERIAL_TX_PORT 14U
#define SERIAL_TX_PIN 0U
#define SERIAL_RX_PORT 14U
#define SERIAL_RX_PIN 1U

#define SERIAL_TX_PC 2U
#define SERIAL_RX_ALTI 0U

#define SERIAL_OVERSAMPLING 16U
#define SERIAL_SAMPLE_POINT 9U

static atomic_bool g_serial_ready = 0;
static atomic_flag g_serial_lock = ATOMIC_FLAG_INIT;

static void serial_unlock(void) { atomic_flag_clear(&g_serial_lock); }

static void serial_lock(void) {
  while (atomic_flag_test_and_set(&g_serial_lock)) {
  }
}

static void serial_config_pins(void) {
  bsp_uc_port_EnableOutput(SERIAL_TX_PORT, SERIAL_TX_PIN, SERIAL_TX_PC,
                           UC_PORT_PL_LEVEL_AL, UC_PORT_PD_SPEED_3);
  bsp_uc_port_EnableInput(SERIAL_RX_PORT, SERIAL_RX_PIN,
                          UC_PORT_INPUT_MODE_TRISTATE);
}

static void serial_config_module(uint32_t baudrate) {
  Ifx_ASCLIN *asc = SERIAL_ASCLIN;
  uint32_t fasc = 100000000U;
  uint32_t prescaler = 1U;
  uint32_t oversample = SERIAL_OVERSAMPLING;

  if (baudrate > 0U) {
    prescaler = (fasc / (baudrate * oversample));
    if (prescaler == 0U) {
      prescaler = 1U;
    }
    if (prescaler > 0U) {
      prescaler -= 1U;
    }
  }

  asc->CLC.B.DISR = 0U;
  while (asc->CLC.B.DISS != 0U) {
  }

  asc->CSR.B.CLKSEL = 1U;
  asc->BITCON.B.PRESCALER = (uint16_t)prescaler;
  asc->BITCON.B.OVERSAMPLING = (uint8_t)(SERIAL_OVERSAMPLING - 1U);
  asc->BITCON.B.SAMPLEPOINT = (uint8_t)SERIAL_SAMPLE_POINT;
  asc->BITCON.B.SM = 0U;

  asc->FRAMECON.B.MODE = 1U;
  asc->FRAMECON.B.STOP = 1U;
  asc->FRAMECON.B.MSB = 0U;
  asc->FRAMECON.B.PEN = 0U;
  asc->FRAMECON.B.ODD = 0U;

  asc->DATCON.B.DATLEN = 7U;

  asc->IOCR.B.ALTI = SERIAL_RX_ALTI;
  asc->TXFIFOCON.B.FLUSH = 1U;
  asc->TXFIFOCON.B.INW = 0U;
  asc->TXFIFOCON.B.FM = 0U;
  asc->TXFIFOCON.B.ENO = 1U;
}

int8_t hardware_init_serial(void) {
  serial_config_pins();
  return 0;
}

int8_t serial_setup(const uint32_t freq) {
  serial_config_module(freq);
  g_serial_ready = 1;
  return 0;
}

int8_t serial_read(uint8_t *restrict const o_buffer,
                   const uint32_t buffer_size) {
  (void)o_buffer;
  (void)buffer_size;
  return -1;
}

static int8_t serial_write_byte(uint8_t byte) {
  Ifx_ASCLIN *asc = SERIAL_ASCLIN;
  if (!g_serial_ready) {
    return -1;
  }

  while (asc->TXFIFOCON.B.FILL >= 16U) {
  }
  asc->TXDATA.U = (uint32_t)byte;
  return 0;
}

int8_t serial_write_raw(const char *const restrict buffer) {
  if (!g_serial_ready) {
    return -1;
  }

  serial_lock();
  for (const char *p = buffer; *p != '\0'; p++) {
    if (serial_write_byte((uint8_t)*p) != 0) {
      serial_unlock();
      return -1;
    }
  }
  serial_unlock();
  return 0;
}

int8_t serial_write_str(const char *const restrict buffer) {
  if (serial_write_raw(buffer) != 0) {
    return -1;
  }
  return serial_write_raw("\r\n");
}

#define TEMPLATE_WRITE_DIG(DIG_TYPE, FORMAT)                                   \
  int8_t serial_write_##DIG_TYPE(const DIG_TYPE __##DIG_TYPE) {                \
    char string[16] = {0};                                                     \
    sprintf(string, FORMAT, __##DIG_TYPE);                                     \
    return serial_write_raw(string);                                           \
  }

TEMPLATE_WRITE_DIG(uint8_t, "%u")
TEMPLATE_WRITE_DIG(uint16_t, "%u")
TEMPLATE_WRITE_DIG(uint32_t, "%u")
TEMPLATE_WRITE_DIG(int8_t, "%d")
TEMPLATE_WRITE_DIG(int16_t, "%d")
TEMPLATE_WRITE_DIG(int32_t, "%d")
TEMPLATE_WRITE_DIG(float, "%0.5f")
