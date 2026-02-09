#include "can_log.h"
#include "../../../../../lib/raceup_board/raceup_board.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include "../../../../../lib/board_dbc/dbc/out_lib/can2/can2.h"
#pragma GCC diagnostic pop 
#include "../../../../core_utility/core_utility.h"

#include <stdint.h>
#include <string.h>

struct CanLog_t{
  time_var_microseconds m_last_sent;
  uint32_t m_brake_pressure[2];
  GpioRead_h m_start_precharge_gpio;
  GpioRead_h m_done_precharge_gpio;
  GpioRead_h m_gpio_scs;
  GpioRead_h m_gpio_drive_button;
  Adc_h m_adc_hydraulic_pressure_front;
  Adc_h m_adc_hydraulic_pressure_rear;
  const uint16_t* p_log_var_amk_status_fl;
  const uint16_t* p_log_var_amk_status_fr;
  const uint16_t* p_log_var_amk_status_rl;
  const uint16_t* p_log_var_amk_status_rr;
  const uint8_t* p_bre_imp;
  struct CanMailbox* p_mailbox_send_car_status;
  struct CanMailbox* p_mailbox_send_hydraulic_pressure;
};

union CanLog_h_t_conv{
  CanLog_h* const hidden;
  struct CanLog_t* const clear;
};

#ifdef DEBUG
char __assert_size_can_log[sizeof(CanLog_h)==sizeof(struct CanLog_t)?+1:-1];
char __assert_align_can_log[_Alignof(CanLog_h)==_Alignof(struct CanLog_t)?+1:-1];
#define CHECK_INIT(self)\
if (!self->p_mailbox_send_car_status)\
{\
  SET_TRACE(CORE_1, INIT_ERROR);\
  return -1;\
}
#else
#define CHECK_INIT(self)
#endif /* ifdef DEBUG */

// Returns Pa brake line pressure (read SP100 Aviorace datasheet)
static uint32_t _getSP100BrakePress(uint32_t adc_reading) {
    float g_adc = 3.3f / (2 << 12);
    float v = ((float)adc_reading) * g_adc * 2;
    float max_press = 100.0; // Bar
    float min_volt = 0.250; // V
    float max_volt = 2.25; // V
    float m = max_press / (max_volt - min_volt);
    
    if (v <= min_volt) {
        return 0;
    }
    
    if (v > min_volt && v < max_volt) {
        return (uint32_t) (m * (v - min_volt));
    }
        
    return (uint32_t) (max_press);
    
}

//public

int8_t can_log_init(CanLog_h* const restrict self)
{
  union CanLog_h_t_conv conv = {self};
  struct CanLog_t* const p_self = conv.clear;
  struct CanNode* can_node = NULL;
  int8_t err=0;

  memset(p_self, 0, sizeof(*p_self));

  {
    LogEntry_h entry =
    {
      .data_mode = __u8__,
      .data_format = UNSIGNED,
      .log_mode = LOG_TELEMETRY | LOG_SD,
      .name = "brake_press_front",
      .data_ptr = &p_self->m_brake_pressure[0],
    };
    while((err=log_add_entry(&entry))<0)
    {
      serial_write_raw("log init brake_press_front failed: ");
      serial_write_int8_t(err);
      serial_write_str("");
      timer_wait(get_tick_from_millis(200));
    }
  }

  {
    LogEntry_h entry_1 =
    {
      .data_mode = __u32__,
      .data_format = UNSIGNED,
      .log_mode = LOG_TELEMETRY | LOG_SD,
      .name = "brake_press_rear",
      .data_ptr = &p_self->m_brake_pressure[1],
    };
    while((err=log_add_entry(&entry_1))<0)
    {
      serial_write_raw("log init brake_press_front failed: ");
      serial_write_int8_t(err);
      serial_write_str("");
      timer_wait(get_tick_from_millis(200));
    }
  }

  p_self->p_log_var_amk_status_fl = (void*) NS_SH(get)(NS_SD(AMK_STATUS_FL));
  p_self->p_log_var_amk_status_rl = (void*) NS_SH(get)(NS_SD(AMK_STATUS_RL));

  p_self->p_log_var_amk_status_fr = (void*) NS_SH(get)(NS_SD(AMK_STATUS_FR));
  p_self->p_log_var_amk_status_rr = (void*) NS_SH(get)(NS_SD(AMK_STATUS_RR));

  p_self->p_bre_imp = (void*) NS_SH(get)(NS_SD(BRE_IMP));

  if (!p_self->p_log_var_amk_status_fl ||
      !p_self->p_log_var_amk_status_fr ||
      !p_self->p_log_var_amk_status_rr ||
      !p_self->p_log_var_amk_status_rl)
  {
    timer_wait(get_tick_from_millis(300));
    goto external_data_ptr_failed;
  }

  if (hardware_init_read_permission_gpio(&p_self->m_start_precharge_gpio, GPIO_AIR_PRECHARGE_INIT)<0)
  {
    goto gpio_precharge_init_failed;
  }

  if (hardware_init_read_permission_gpio(&p_self->m_done_precharge_gpio, GPIO_AIR_PRECHARGE_DONE)<0)
  {
    goto gpio_precharge_done_failed;
  }

  if (hardware_init_read_permission_gpio(&p_self->m_gpio_drive_button, GPIO_RTD_BUTTON)<0)
  {
    goto gpio_rtd_button_failed;
  }

  if (hardware_init_read_permission_gpio(&p_self->m_gpio_scs, GPIO_SCS)<0)
  {
    goto gpio_scs_failed;
  }

  if (NS_ADC(init)(&p_self->m_adc_hydraulic_pressure_front, NS_ADP(BRAKE_PRESSURE_FRONT))<0)
  {
    goto adc_front_failed;
  }

  if (NS_ADC(init)(&p_self->m_adc_hydraulic_pressure_rear, NS_ADP(BRAKE_PRESSURE_REAR))<0)
  {
    goto adc_rear_failed;
  }

  ACTION_ON_CAN_NODE(CAN_GENERAL, can_node)
  {
    p_self->p_mailbox_send_car_status =
      hardware_get_mailbox_single_mex(
          can_node,
          SEND_MAILBOX,
          CAN_ID_CARSTATUS,
          (uint16_t) message_dlc_can2(CAN_ID_CARSTATUS));
  }

  if (!p_self->p_mailbox_send_car_status)
  {
    goto mailbox_car_status_failed;
  }

  ACTION_ON_CAN_NODE(CAN_GENERAL, can_node)
  {
    p_self->p_mailbox_send_hydraulic_pressure=
      hardware_get_mailbox_single_mex(
          can_node,
          SEND_MAILBOX,
          CAN_ID_HYDRAULICPRESSURE,
          (uint16_t) message_dlc_can2(CAN_ID_HYDRAULICPRESSURE));
  }

  if (!p_self->p_mailbox_send_hydraulic_pressure)
  {
    goto mailbox_hyd_press_failed;
  }



  return 0;

  err--;
  hardware_free_mailbox_can(&p_self->p_mailbox_send_hydraulic_pressure);
mailbox_hyd_press_failed:
  err--;
  hardware_free_mailbox_can(&p_self->p_mailbox_send_car_status);
mailbox_car_status_failed:
  err--;
  NS_ADC(destroy)(&p_self->m_adc_hydraulic_pressure_rear);
adc_rear_failed:
  err--;
  NS_ADC(destroy)(&p_self->m_adc_hydraulic_pressure_front);
adc_front_failed:
  err--;
  gpio_read_destroy(&p_self->m_gpio_scs);
gpio_scs_failed:
  err--;
  gpio_read_destroy(&p_self->m_gpio_drive_button);
gpio_rtd_button_failed:
  err--;
  gpio_read_destroy(&p_self->m_done_precharge_gpio);
gpio_precharge_done_failed:
  err--;
  gpio_read_destroy(&p_self->m_start_precharge_gpio);
gpio_precharge_init_failed:
  err--;
external_data_ptr_failed:
  err--;

  return err;
}

int8_t can_log_update(CanLog_h* const restrict self)
{
  union CanLog_h_t_conv conv = {self};
  struct CanLog_t* const p_self = conv.clear;
  can_obj_can2_h_t o2 = {0};
  uint64_t payload = 0;
  int8_t err=0;

  CHECK_INIT(p_self);

  ACTION_ON_FREQUENCY(p_self->m_last_sent, get_tick_from_millis(100))
  {
    //HACK: look amk.c AMK_Actual_Values_1 to check if the position checked with the shift are right
    o2.can_0x065_CarStatus.HV= 
      !!((*p_self->p_log_var_amk_status_fl >> 8) & (1<<3)) ||
      !!((*p_self->p_log_var_amk_status_fr >> 8) & (1<<3)) ||
      !!((*p_self->p_log_var_amk_status_rl >> 8) & (1<<3)) ||
      !!((*p_self->p_log_var_amk_status_rr >> 8) & (1<<3));


    o2.can_0x065_CarStatus.AIR1= !!gpio_read_state(&p_self->m_start_precharge_gpio);
    o2.can_0x065_CarStatus.precharge = !!gpio_read_state(&p_self->m_done_precharge_gpio);
    o2.can_0x065_CarStatus.AS_NODE= !as_node_read_get();
    o2.can_0x065_CarStatus.SCS= !gpio_read_state(&p_self->m_gpio_scs);
    o2.can_0x065_CarStatus.rtd_req = !!gpio_read_state(&p_self->m_gpio_drive_button);
    o2.can_0x065_CarStatus.RunningStatus = (uint8_t) global_running_status_get();
    o2.can_0x065_CarStatus.speed = (uint8_t) car_speed_get();
    o2.can_0x065_CarStatus.bre_impl = *p_self->p_bre_imp;

    pack_message_can2(&o2, CAN_ID_CARSTATUS, &payload);
    err += hardware_mailbox_send(p_self->p_mailbox_send_car_status, payload);

    p_self->m_brake_pressure[1]= _getSP100BrakePress(NS_ADC(read)(&p_self->m_adc_hydraulic_pressure_rear));
    p_self->m_brake_pressure[0]= _getSP100BrakePress(NS_ADC(read)(&p_self->m_adc_hydraulic_pressure_front));

    o2.can_0x108_HydraulicPressure.PressFront = p_self->m_brake_pressure[0];
    o2.can_0x108_HydraulicPressure.PressRear = p_self->m_brake_pressure[1];

    pack_message_can2(&o2, CAN_ID_HYDRAULICPRESSURE, &payload);
    err += hardware_mailbox_send(p_self->p_mailbox_send_hydraulic_pressure, payload);
  }

  return err;
}

int8_t can_log_destroy(CanLog_h* const restrict self)
{
  union CanLog_h_t_conv conv = {self};
  struct CanLog_t* const p_self = conv.clear;

  hardware_free_mailbox_can(&p_self->p_mailbox_send_hydraulic_pressure);
  hardware_free_mailbox_can(&p_self->p_mailbox_send_car_status);
  NS_ADC(destroy)(&p_self->m_adc_hydraulic_pressure_rear);
  NS_ADC(destroy)(&p_self->m_adc_hydraulic_pressure_front);
  gpio_read_destroy(&p_self->m_gpio_scs);
  gpio_read_destroy(&p_self->m_gpio_drive_button);
  gpio_read_destroy(&p_self->m_done_precharge_gpio);
  gpio_read_destroy(&p_self->m_start_precharge_gpio);

  return 0;
}
