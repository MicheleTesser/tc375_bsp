#include "FreeRTOS.h"
#include "bsp.h"
#include "driver/compontent.h"
#include "driver/flash_memory.h"
#include "task.h"

/* ================================================================================================
 * SYMBOLS
 * ==============================================================================================*/

#define LED_TASK_PERIOD_MS 250U
#define OUTPUT_BLINK_PERIOD_MS 500U
#define INPUT_ADC_PERIOD_MS 1000U
#define CAN_TASK_PERIOD_MS 5U
#define ETHERNET_UPDATE_PERIOD_MS 10U
#define UDP_SEND_PERIOD_MS 1000U
#define CAN_FLASH_BANK_ID 0U

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

/* ================================================================================================
 * FUNCTIONS
 * ==============================================================================================*/

static Gpio_h g_debug_leds[3];
static Gpio_h g_output_leds[8];
static GpioRead_h g_inputs[3];
static Adc_h g_adc[2];
static EthernetNodeIpv4_h g_eth_node;
static char g_udp_addr[] = "192.168.0.100";
static FlashMemoryBank_h g_flash_bank;
static uint8_t g_flash_ready = 0U;

static void vDebugLedTask(void *pvParameters) {
  Gpio_h *led = (Gpio_h *)pvParameters;
  TickType_t lastWake = xTaskGetTickCount();

  for (;;) {
    gpio_toggle(led);
    vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(LED_TASK_PERIOD_MS));
  }
}

static void vOutputsTask(void *pvParameters) {
  (void)pvParameters;
  TickType_t lastWake = xTaskGetTickCount();

  for (;;) {
    for (size_t i = 0; i < ARRAY_SIZE(g_output_leds); i++) {
      gpio_toggle(&g_output_leds[i]);
    }
    vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(OUTPUT_BLINK_PERIOD_MS));
  }
}

static void vInputAdcTask(void *pvParameters) {
  (void)pvParameters;
  TickType_t lastWake = xTaskGetTickCount();

  for (;;) {
    uint32_t rtd_button = (uint32_t)gpio_read_state(&g_inputs[0]);
    uint32_t air_precharge_init = (uint32_t)gpio_read_state(&g_inputs[1]);
    uint32_t air_precharge_done = (uint32_t)gpio_read_state(&g_inputs[2]);

    AdcValue brake_front = NS_ADC(read)(&g_adc[0]);
    AdcValue brake_rear = NS_ADC(read)(&g_adc[1]);

    serial_write_str("IN: RTD=");
    serial_write_uint32_t(rtd_button);
    serial_write_str(" AIR_INIT=");
    serial_write_uint32_t(air_precharge_init);
    serial_write_str(" AIR_DONE=");
    serial_write_uint32_t(air_precharge_done);
    serial_write_str(" ADC_F=");
    serial_write_uint32_t(brake_front);
    serial_write_str(" ADC_R=");
    serial_write_uint32_t(brake_rear);
    if (g_flash_ready) {
      uint32_t stored[TOT_NUM_OF_PARTS] = {0};
      (void)flash_memory_read_bank(&g_flash_bank, stored);
      serial_write_str(" CAN=");
      serial_write_uint32_t(stored[1]);
      serial_write_str("_");
      serial_write_uint32_t(stored[0]);
    }
    serial_write_str("\r\n");

    vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(INPUT_ADC_PERIOD_MS));
  }
}

static void vCanEchoTask(void *pvParameters) {
  (void)pvParameters;
  TickType_t lastWake = xTaskGetTickCount();
  struct CanNode *node = NULL;
  struct CanMailbox *rx_mb = NULL;
  struct CanMailbox *tx_mb = NULL;
  CanMessage rx = {0};
  uint32_t flash_words[TOT_NUM_OF_PARTS] = {0};

  ACTION_ON_CAN_NODE(CAN_DEBUG, node) {
    rx_mb = hardware_get_mailbox_single_mex(node, RECV_MAILBOX, 0x100U, 8U);
    tx_mb = hardware_get_mailbox_single_mex(node, SEND_MAILBOX, 0x101U, 8U);
  }

  for (;;) {
    if (hardware_mailbox_read(rx_mb, &rx) == 1) {
      (void)hardware_mailbox_send(tx_mb, rx.full_word);
      flash_words[0] = (uint32_t)(rx.full_word & 0xFFFFFFFFU);
      flash_words[1] = (uint32_t)((rx.full_word >> 32U) & 0xFFFFFFFFU);
      if (g_flash_ready) {
        (void)flash_memory_write_bank(&g_flash_bank, flash_words);
      }
    }
    vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(CAN_TASK_PERIOD_MS));
  }
}

static void vEthernetUdpTask(void *pvParameters) {
  (void)pvParameters;
  TickType_t lastWake = xTaskGetTickCount();
  TickType_t lastSend = lastWake;
  static const char msg[] = "Hello from TC375 UDP";
  const UdpIpv4Mex packet = {
      .raw_data = msg,
      .data_length = (uint16_t)(sizeof(msg) - 1U),
  };

  for (;;) {
    hardware_ethernet_update();

    if ((xTaskGetTickCount() - lastSend) >= pdMS_TO_TICKS(UDP_SEND_PERIOD_MS)) {
      (void)hardware_ethernet_udp_send(&g_eth_node, &packet);
      lastSend = xTaskGetTickCount();
    }

    vTaskDelayUntil(&lastWake, pdMS_TO_TICKS(ETHERNET_UPDATE_PERIOD_MS));
  }
}
static void init_io(void) {
  hardware_init_gpio(&g_debug_leds[0], GPIO_CORE_0_ALIVE_BLINK, 1U);
  hardware_init_gpio(&g_debug_leds[1], GPIO_CORE_1_ALIVE_BLINK, 1U);
  hardware_init_gpio(&g_debug_leds[2], GPIO_CORE_2_ALIVE_BLINK, 1U);

  hardware_init_gpio(&g_output_leds[0], GPIO_RTD_ASSI_SOUND, 1U);
  hardware_init_gpio(&g_output_leds[1], GPIO_SCS, 1U);
  hardware_init_gpio(&g_output_leds[2], GPIO_AS_NODE, 1U);
  hardware_init_gpio(&g_output_leds[3], GPIO_RTD_BUTTON_LED, 1U);
  hardware_init_gpio(&g_output_leds[4], GPIO_ASSI_LIGHT_BLUE, 1U);
  hardware_init_gpio(&g_output_leds[5], GPIO_ASSI_LIGHT_YELLOW, 1U);
  hardware_init_gpio(&g_output_leds[6], GPIO_AMS, 1U);
  hardware_init_gpio(&g_output_leds[7], GPIO_IMD, 1U);

  hardware_init_read_permission_gpio(&g_inputs[0], GPIO_RTD_BUTTON);
  hardware_init_read_permission_gpio(&g_inputs[1], GPIO_AIR_PRECHARGE_INIT);
  hardware_init_read_permission_gpio(&g_inputs[2], GPIO_AIR_PRECHARGE_DONE);

  NS_ADC(init)(&g_adc[0], NS_ADP(BRAKE_PRESSURE_FRONT));
  NS_ADC(init)(&g_adc[1], NS_ADP(BRAKE_PRESSURE_REAR));
}

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: shared_main()
 *
 * Shared entry point if there are more core present in uC derivative.
 * It is called by crt0 startup code after crt0 finishes 'C' runtime environment
 * initialization.
 *
 * To get a HW platform in an expected state, the initialization is done within
 * Crt0PostInit hook called before entering shared_main from crt0.
 * ------------------------------------------------------------------------------------------------
 */
void shared_main(void) {
  /* Use BSP API to get current CoreId */
  uint32_t coreId = bsp_uc_core_GetCurrentCore();
  if (coreId == UC_RESET_CORE) {
    (void)hardware_init_serial();
    (void)serial_setup(115200U);
    hardware_init_can(CAN_DEBUG, _500_KBYTE_S_);
    (void)hardware_init_flash_memory();
    if (flash_memory_req_bank(&g_flash_bank, CAN_FLASH_BANK_ID) == 0) {
      g_flash_ready = 1U;
    }

    (void)hardware_ethernet_udp_init(
        &g_eth_node, &(IpAddrIpV4Port){.addr = g_udp_addr, .port = 5000U});
    init_io();

    configASSERT(xTaskCreate(vDebugLedTask, "DBG0", configMINIMAL_STACK_SIZE,
                             &g_debug_leds[0], tskIDLE_PRIORITY + 2,
                             NULL) == pdPASS);
    configASSERT(xTaskCreate(vDebugLedTask, "DBG1", configMINIMAL_STACK_SIZE,
                             &g_debug_leds[1], tskIDLE_PRIORITY + 2,
                             NULL) == pdPASS);
    configASSERT(xTaskCreate(vDebugLedTask, "DBG2", configMINIMAL_STACK_SIZE,
                             &g_debug_leds[2], tskIDLE_PRIORITY + 2,
                             NULL) == pdPASS);

    configASSERT(xTaskCreate(vOutputsTask, "OUT", configMINIMAL_STACK_SIZE,
                             NULL, tskIDLE_PRIORITY + 1, NULL) == pdPASS);
    configASSERT(xTaskCreate(vInputAdcTask, "INADC", configMINIMAL_STACK_SIZE,
                             NULL, tskIDLE_PRIORITY + 1, NULL) == pdPASS);
    configASSERT(xTaskCreate(vCanEchoTask, "CAN", configMINIMAL_STACK_SIZE,
                             NULL, tskIDLE_PRIORITY + 1, NULL) == pdPASS);
    configASSERT(xTaskCreate(vEthernetUdpTask, "UDP", configMINIMAL_STACK_SIZE,
                             NULL, tskIDLE_PRIORITY + 1, NULL) == pdPASS);
  }

  bsp_uc_intc_EnableExternalInterrupts();
  vTaskStartScheduler();

  for (;;) {
  }
}

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: Crt0PreInit()
 *
 *  Crt0 Hook before memory clear & copy operation take place.
 *  The code cannot rely on initialized and cleared global data !
 * ------------------------------------------------------------------------------------------------
 */
void Crt0PreInit(void) {
  /* Use BSP API to get current CoreId */
  uint32_t coreId = bsp_uc_core_GetCurrentCore();

  /* disable Safety WATCHDOG by the RESET Core only */
  if (coreId == UC_RESET_CORE) {
    bsp_uc_scu_wdt_DisableSafetyWatchdog();
  }

  /* disable Core WATCHDOG */
  bsp_uc_scu_wdt_DisableCpuWatchdog();

  /* enable Core instruction cache */
  bsp_uc_core_EnableICache();
}

/* ------------------------------------------------------------------------------------------------
 * FUNCTION: Crt0PostInit()
 *
 * Crt0 Hook after complete crt0 initialization, just before shared_main() entry
 * point is called. It is the place of uC shared HW resource initialization.
 * ------------------------------------------------------------------------------------------------
 */
void Crt0PostInit(void) {
  /* Use BSP API to get current CoreId */
  uint32_t coreId = bsp_uc_core_GetCurrentCore();

  /* Initialization of shared resources by RESET core */
  if (coreId == UC_RESET_CORE) {
    /* setup clock system of uC */
    bsp_uc_InitClock();

    /* disable eventual external WATCHDOG on the board (not available on
     * litekit)*/
#if !defined LITEKIT_TC375_V2
    bsp_board_wdg_Disable();
#endif

    /* start other cores */
    bsp_uc_core_StartAllCores();
  }

  /* setup ISR sub-system on each core */
  bsp_isr_Init();
}
