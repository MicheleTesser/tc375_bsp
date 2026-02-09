#include "driver_input.h"
#include "../../../core_utility/core_utility.h"
#include "../../../../lib/raceup_board/raceup_board.h"
#include "../../../../lib/board_dbc/dbc/out_lib/can3/can3.h"

#include <stdint.h>
#include <string.h>
#include <stdatomic.h>

//private

struct DriverInput_t{
  enum DRIVER current_driver;
  struct{
    GpioRead_h gpio_rtd_button;
    uint8_t dv_rtd_input_request:1;
  }Rtd_request;
  float m_active_throttle;
  DriverInputReader_h m_driver_input_reader;
  CarMissionReader_h* p_car_mission;
};

union DriverInput_h_t_conv{
  DriverInput_h* const restrict hidden;
  struct DriverInput_t* const restrict clear;
};

union DriverInput_h_t_conv_const{
  const DriverInput_h* const restrict hidden;
  const struct DriverInput_t* const restrict clear;
};

union DriverInputConv{
  DriverInput_h* const hidden;
  struct DriverInput_t* const clear;
};

#ifdef DEBUG
const uint8_t __assert_driver_input_size[sizeof(DriverInput_h) == sizeof(struct DriverInput_t)? 1 : -1];
const uint8_t __assert_driver_input_align[_Alignof(DriverInput_h) == _Alignof(struct DriverInput_t)? 1 : -1];
#endif /* ifdef DEBUG */

static struct{
  uint8_t m_roof_imp;
  FlashMemoryBank_h m_roof_bank;
  _Atomic uint8_t m_init_done;
  uint8_t m_max_imps;
  uint8_t m_bre_imp;
  time_var_microseconds m_start_imp;
}G_DATA;

static int8_t _post_update_roof_imp(const char* const restrict name, const void* const ptr)
{
  (void)name;
  uint32_t data[2] = {0};
  int8_t err=0;

  data[0] = *(uint8_t *)ptr;

  if((err=flash_memory_write_bank(&G_DATA.m_roof_bank, data)<0))
  {
    serial_write_raw("updata bank roof imp failed: ");
    serial_write_int8_t(err);
    serial_write_str("");
  }

  return 0;
}

//public

int8_t driver_input_init(DriverInput_h* const restrict self, 
    CarMissionReader_h* const restrict p_car_mission)
{
  union DriverInputConv d_conv = {self};
  struct DriverInput_t* const p_self = d_conv.clear;
  int8_t err=0;

  memset(p_self, 0, sizeof(*p_self));

  if (!atomic_exchange(&G_DATA.m_init_done, 1U))
  {
    uint32_t data[2] = {0};

    while((err=flash_memory_req_bank(&G_DATA.m_roof_bank, 5))<0)
    {
      serial_write_raw("init bank roof imp failed: ");
      serial_write_int8_t(err);
      serial_write_str("");
      timer_wait(get_tick_from_millis(200));
    }

    while((err=flash_memory_read_bank(&G_DATA.m_roof_bank, data))<0)
    {
      serial_write_raw("read bank roof imp failed: ");
      serial_write_int8_t(err);
      serial_write_str("");
      timer_wait(get_tick_from_millis(200));
    }
    G_DATA.m_roof_imp = (uint8_t) data[0];

    if (!G_DATA.m_roof_imp)
    {
      G_DATA.m_roof_imp = 20;
      (void)_post_update_roof_imp(NULL, &G_DATA.m_roof_imp);
    }

    const char roof_imp[6] = "rimp";
    while((err=system_settings_push_var(
            DPS_TYPES_UINT8_T,
            &G_DATA.m_roof_imp, 
            _post_update_roof_imp, 
            roof_imp))<0)
    {
      serial_write_raw("system settings roof imp failed: ");
      serial_write_int8_t(err);
      serial_write_str("");
      timer_wait(get_tick_from_millis(200));
    }
    LogEntry_h entry =
    {
      .data_format = UNSIGNED,
      .data_mode = __u32__,
      .data_ptr = &G_DATA.m_bre_imp,
      .log_mode = LOG_TELEMETRY | LOG_SD,
      .name = "driver_bre_impl",
    };

    while((err=log_add_entry(&entry))<0)
    {
      serial_write_raw("pushing var log impls failed: ");
      serial_write_int8_t(err);
      serial_write_str("");
      timer_wait(get_tick_from_millis(200));
    }

    entry.data_format = FLOAT;
    entry.data_mode = __float__;
    entry.log_mode = LOG_SD | LOG_TELEMETRY;
    entry.data_ptr = &p_self->m_active_throttle;
    strncpy(entry.name, "throttle_active", sizeof(entry.name)-1);

    while((err=log_add_entry(&entry))<0)
    {
      serial_write_raw("pushing var log throttle_active failed: ");
      serial_write_int8_t(err);
      serial_write_str("");
      timer_wait(get_tick_from_millis(200));
    }
  }


  if (hardware_init_read_permission_gpio(&p_self->Rtd_request.gpio_rtd_button, GPIO_RTD_BUTTON)<0)
  {
    goto init_gpio_rtd_failed;
  }

  if (driver_input_reader_init(&p_self->m_driver_input_reader) < 0)
  {
    goto init_driver_iput_reader_failed; 
  }

  p_self->current_driver = DRIVER_NONE;
  p_self->p_car_mission = p_car_mission;

  NS_SH(update)(NS_SD(BRE_IMP), (uint32_t) &G_DATA.m_bre_imp);

  return err;

  err--;
  driver_input_reader_destroy(&p_self->m_driver_input_reader);
init_driver_iput_reader_failed:
  err--;
  gpio_read_destroy(&p_self->Rtd_request.gpio_rtd_button);
init_gpio_rtd_failed:
  err--;

  return err;
}

float giei_driver_input_get(const DriverInput_h* const restrict self,
    const enum INPUT_TYPES input_type)
{
  const union DriverInput_h_t_conv_const conv = {self};
  const struct DriverInput_t* const p_self = conv.clear;
  uint8_t enable_thottle=1;

  if (input_type == THROTTLE && G_DATA.m_start_imp)
  {
    enable_thottle = 0;
  }


  switch (p_self->current_driver) {
    case DRIVER_HUMAN:
    case DRIVER_EMBEDDED:
      return enable_thottle * driver_input_reader_get(&p_self->m_driver_input_reader, p_self->current_driver, input_type);
    default:
      return -1;
  }

  return 0;
}

int8_t giei_driver_input_rtd_request(const DriverInput_h* const restrict self)
{
  const union DriverInput_h_t_conv_const conv = {self};
  const struct DriverInput_t* const p_self = conv.clear;

  switch (p_self->current_driver) {
    case DRIVER_HUMAN:
      return gpio_read_state(&p_self->Rtd_request.gpio_rtd_button);
    case DRIVER_EMBEDDED:
      return NS_SH(get)(NS_SD(GO_STATUS))==1;
    case DRIVER_NONE:
      return 0;
    default:
      SET_TRACE(CORE_0,UNREACHABLE_CODE);
      return -1;
  }
}

int8_t giei_driver_input_update(DriverInput_h* const restrict self )
{
  union DriverInput_h_t_conv conv = {self};
  struct DriverInput_t* const restrict p_self = conv.clear;
  float throttle =0;
  float brake =0;
  uint8_t check_imp=0;

  check_imp = driver_input_reader_update(&p_self->m_driver_input_reader)>0;
  switch (NS_CAR_MISS(get_current_mission)(p_self->p_car_mission))
  {
    case NS_CAR_MISS(NONE):
      p_self->current_driver = DRIVER_NONE;
      break;
    case NS_CAR_MISS(HUMAN):
      p_self->current_driver = DRIVER_HUMAN;
      break;
    case NS_CAR_MISS(DV_ACCELERATION):
    case NS_CAR_MISS(DV_SKIDPAD):
    case NS_CAR_MISS(DV_AUTOCROSS):
    case NS_CAR_MISS(DV_TRACKDRIVE):
    case NS_CAR_MISS(DV_EBS_TEST):
    case NS_CAR_MISS(DV_INSPECTION):
      p_self->current_driver = DRIVER_EMBEDDED;
      break;
    case NS_CAR_MISS(__NUM_OF__):
      SET_TRACE(CORE_0, UNREACHABLE_CODE);
      return -1;
      break;
  }

  if (check_imp)
  {
    throttle = driver_input_reader_get(&p_self->m_driver_input_reader, p_self->current_driver, THROTTLE);
    brake = driver_input_reader_get(&p_self->m_driver_input_reader, p_self->current_driver, BRAKE);

    if (throttle<0)
    {
      return 0;
    }

    if ((throttle > G_DATA.m_roof_imp && brake > G_DATA.m_roof_imp) ||
        (throttle > G_DATA.m_max_imps && G_DATA.m_start_imp))
    {
      if (G_DATA.m_max_imps< (1u << (sizeof(G_DATA.m_max_imps) * 8u)) -1)
      {
        G_DATA.m_max_imps++;
      }
      if (G_DATA.m_max_imps > 8)
      {
        G_DATA.m_start_imp = timer_time_now();
      }
    }
    else if(timer_time_now() - G_DATA.m_start_imp > get_tick_from_millis(5000))
    {
      G_DATA.m_start_imp = 0;
      G_DATA.m_max_imps=0;
    }
    G_DATA.m_bre_imp = !!G_DATA.m_start_imp;

    p_self->m_active_throttle = (!G_DATA.m_bre_imp) * throttle;
  }


  return 0;
}

int8_t driver_input_destroy(DriverInput_h* restrict self)
{
  const union DriverInput_h_t_conv conv = {self};
  struct DriverInput_t* const p_self = conv.clear;

  driver_input_reader_destroy(&p_self->m_driver_input_reader);
  gpio_read_destroy(&p_self->Rtd_request.gpio_rtd_button);

  memset(p_self, 0, sizeof(*p_self));
  return 0;
}
