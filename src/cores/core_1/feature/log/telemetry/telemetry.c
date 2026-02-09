#include "telemetry.h"
#include "../../../../../lib/raceup_board/components/ethernet.h"
#include "../../../../core_utility/core_utility.h"
#include "json_builder/json_builder.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct
{
  enum DATA_MODE m_data_type;
  const void* p_data;
  const char* const m_name;
  DataFormat data_format;
}JsonCell;

#define NEW_EMPTY_CELL(name) \
  { .m_data_type = __u8__, .p_data = NULL, .m_name =name, .data_format = UNSIGNED},

static JsonCell JSON_1[32] =
{
  //INFO: amk
  NEW_EMPTY_CELL("amk_status_fl")
  NEW_EMPTY_CELL("amk_target_velocity_fl")
  NEW_EMPTY_CELL("amk_actual_velocity_fl")
  NEW_EMPTY_CELL("amk_torque_current_fl")
  NEW_EMPTY_CELL("amk_magnetizing_current_fl")
  NEW_EMPTY_CELL("amk_status_fr")
  NEW_EMPTY_CELL("amk_target_velocity_fr")
  NEW_EMPTY_CELL("amk_actual_velocity_fr")
  NEW_EMPTY_CELL("amk_torque_current_fr")
  NEW_EMPTY_CELL("amk_magnetizing_current_fr")
  NEW_EMPTY_CELL("amk_status_rr")
  NEW_EMPTY_CELL("amk_target_velocity_rr")
  NEW_EMPTY_CELL("amk_actual_velocity_rr")
  NEW_EMPTY_CELL("amk_torque_current_rr")
  NEW_EMPTY_CELL("amk_magnetizing_current_rr")
  NEW_EMPTY_CELL("amk_status_rl")
  NEW_EMPTY_CELL("amk_target_velocity_rl")
  NEW_EMPTY_CELL("amk_actual_velocity_rl")
  NEW_EMPTY_CELL("amk_torque_current_rl")
  NEW_EMPTY_CELL("amk_magnetizing_current_rl")
  NEW_EMPTY_CELL("amk_error_info_rr")
  NEW_EMPTY_CELL("amk_error_info_rl")
  NEW_EMPTY_CELL("amk_torque_limit_positive_fl")
  NEW_EMPTY_CELL("amk_torque_limit_negative_fl")
  NEW_EMPTY_CELL("amk_torque_limit_positive_fr")
  NEW_EMPTY_CELL("amk_torque_limit_negative_fr")
  NEW_EMPTY_CELL("amk_torque_limit_positive_rr")
  NEW_EMPTY_CELL("amk_torque_limit_negative_rr")
  NEW_EMPTY_CELL("amk_torque_limit_positive_rl")
  NEW_EMPTY_CELL("amk_torque_limit_negative_rl")
  NEW_EMPTY_CELL("amk_error_info_fl")
  NEW_EMPTY_CELL("amk_error_info_fr")
};


static JsonCell JSON_2[32] =
{
  //INFO: lv temp
  NEW_EMPTY_CELL("max_lv_temp")
  NEW_EMPTY_CELL("min_lv_temp")
  NEW_EMPTY_CELL("avg_lv_temp")

  //INFO: hv temp
  NEW_EMPTY_CELL("max_hv_temp")
  NEW_EMPTY_CELL("min_hv_temp")
  NEW_EMPTY_CELL("avg_hv_temp")

  //INFO: temps 
  NEW_EMPTY_CELL("amk_temp_motor_fl")
  NEW_EMPTY_CELL("amk_temp_inverter_fl")
  NEW_EMPTY_CELL("amk_temp_igbt_fl")
  NEW_EMPTY_CELL("amk_temp_motor_fr")
  NEW_EMPTY_CELL("amk_temp_inverter_fr")
  NEW_EMPTY_CELL("amk_temp_igbt_fr")
  NEW_EMPTY_CELL("amk_temp_motor_rr")
  NEW_EMPTY_CELL("amk_temp_inverter_rr")
  NEW_EMPTY_CELL("amk_temp_igbt_rr")
  NEW_EMPTY_CELL("amk_temp_motor_rl")
  NEW_EMPTY_CELL("amk_temp_inverter_rl")
  NEW_EMPTY_CELL("amk_temp_igbt_rl")

  NEW_EMPTY_CELL("temp_post_rr")
  NEW_EMPTY_CELL("temp_post_inv_l")
  NEW_EMPTY_CELL("temp_post_rl")
  NEW_EMPTY_CELL("temp_post_rad_r")
  NEW_EMPTY_CELL("temp_post_inv_r")
  NEW_EMPTY_CELL("temp_post_emb")
  NEW_EMPTY_CELL("temp_post_fr")
  NEW_EMPTY_CELL("temp_post_rad_l")
  NEW_EMPTY_CELL("temp_pump_left")
  NEW_EMPTY_CELL("temp_pump_right")
  NEW_EMPTY_CELL("temp_post_fl")


};

static JsonCell JSON_3[32] =
{
  //INFO: bms lv
  NEW_EMPTY_CELL("max_lv_volt")
  NEW_EMPTY_CELL("min_lv_volt")
  NEW_EMPTY_CELL("avg_lv_volt")
  NEW_EMPTY_CELL("tot_lv_volt")
  NEW_EMPTY_CELL("lv_lem_current")

  //INFO: bms hv 
  NEW_EMPTY_CELL("max_hv_volt")
  NEW_EMPTY_CELL("min_hv_volt")
  NEW_EMPTY_CELL("avg_hv_volt")
  NEW_EMPTY_CELL("hv_soc")
  NEW_EMPTY_CELL("total_power")
  NEW_EMPTY_CELL("lem_current")
  NEW_EMPTY_CELL("bms_error_map")
  NEW_EMPTY_CELL("car_voltage")

  //INFO: imu
  NEW_EMPTY_CELL("acceleration_x")
  NEW_EMPTY_CELL("acceleration_y")
  NEW_EMPTY_CELL("acceleration_z")
  NEW_EMPTY_CELL("omega_x")
  NEW_EMPTY_CELL("omega_y")
  NEW_EMPTY_CELL("omega_z")

  //INFO: fans
  NEW_EMPTY_CELL("current_fanbattl")
  NEW_EMPTY_CELL("current_fanbattr")
  NEW_EMPTY_CELL("current_fanradl")
  NEW_EMPTY_CELL("current_fanradr")

  //INFO: currents
  NEW_EMPTY_CELL("current_pumpr")
  NEW_EMPTY_CELL("current_pumpl")
};

static JsonCell JSON_4[32] =
{
  //INFO: pressures
  NEW_EMPTY_CELL("brake_press_front")
  NEW_EMPTY_CELL("brake_press_rear")

  //INFO: driver info
  NEW_EMPTY_CELL("driver_pot_impl")
  NEW_EMPTY_CELL("driver_bre_impl")
  NEW_EMPTY_CELL("throttle_human")
  NEW_EMPTY_CELL("throttle_dv")
  NEW_EMPTY_CELL("throttle_active")
  NEW_EMPTY_CELL("steering_angle")
  NEW_EMPTY_CELL("brake")
  NEW_EMPTY_CELL("actual_velocity_kmh")

  //INFO: susps
  NEW_EMPTY_CELL("suspensions_rl")
  NEW_EMPTY_CELL("suspensions_fl")
  NEW_EMPTY_CELL("suspensions_fr")
  NEW_EMPTY_CELL("suspensions_rr")
};

static JsonCell JSON_5[32] =
{
  //INFO: Cs 
  NEW_EMPTY_CELL("CS_yaw_moment")
  NEW_EMPTY_CELL("CS_yr_ref")
  NEW_EMPTY_CELL("CS_vx")
  NEW_EMPTY_CELL("CS_delta_torque_fl")
  NEW_EMPTY_CELL("CS_delta_torque_fr")
  NEW_EMPTY_CELL("CS_delta_torque_rl")
  NEW_EMPTY_CELL("CS_delta_torque_rr")
};


static JsonCell JSON_6[32] =
{
  //INFO: Frequency Analysis
  NEW_EMPTY_CELL("FREQ_core_0_delay")
  NEW_EMPTY_CELL("FREQ_core_1_delay")
  NEW_EMPTY_CELL("FREQ_core_2_delay")

  NEW_EMPTY_CELL("FREQ_core_0_duration")
  NEW_EMPTY_CELL("FREQ_core_1_duration")
  NEW_EMPTY_CELL("FREQ_core_2_duration")

  NEW_EMPTY_CELL("FREQ_compute_power_delay")
  NEW_EMPTY_CELL("FREQ_compute_power_duration")
};

#define ACTIVE_JSON\
  X(JSON_1)\
  X(JSON_2)\
  X(JSON_3)\
  X(JSON_4)\
  X(JSON_5)\
  X(JSON_6)\

#define X(J) J,
  JsonCell* JSON_ARRAYS[] =
{
  ACTIVE_JSON
};
#undef X

#define X(J) sizeof(J) / sizeof(J[0]),
const uint32_t JSON_ARRAY_SIZES[] = 
{
  ACTIVE_JSON
};
#undef X

const uint8_t JSON_ARRAY_COUNT = (sizeof(JSON_ARRAYS) / sizeof(JSON_ARRAYS[0]));

struct LogTelemetry_t{
  EthernetNodeIpv4_h p_ethernet_udp_telemetry;
  time_var_microseconds m_timestamp;
};

union LogTelemetry_h_t_conv{
  LogTelemetry_h* const restrict hidden;
  struct LogTelemetry_t* const restrict clear;
};


#ifdef DEBUG
char __assert_size_telemetry[(sizeof(LogTelemetry_h) == sizeof(struct LogTelemetry_t))? 1:-1];
char __assert_align_telemetry[(_Alignof(LogTelemetry_h) == _Alignof(struct LogTelemetry_t))? 1:-1];
#endif // DEBUG

#define FOR_EACH_JSON(p_json) for(const char*** p_json = &JSON_ARRAYS[0];*p_json;p_json++)

#define FOR_EACH_JSON_FIELD(p_field, p_json)\
  for(const char* p_field=*p_json[0];*p_field;++p_field)


static JsonCell* _search_json_for_log_var(const char* var_name)
{
  for (uint8_t json_num=0;json_num<JSON_ARRAY_COUNT;++json_num)
  {
    for(uint32_t json_line=0;json_line<JSON_ARRAY_SIZES[json_num];++json_line)
    {
      if (JSON_ARRAYS[json_num][json_line].m_name && !strcmp(JSON_ARRAYS[json_num][json_line].m_name, var_name))
      {
        return &JSON_ARRAYS[json_num][json_line];
      }
    }
  }

  return NULL;
}

//public

int8_t log_telemetry_init(LogTelemetry_h* const restrict self )
{
  union LogTelemetry_h_t_conv conv = {self};
  struct LogTelemetry_t* const restrict p_self = conv.clear;
  int8_t err=0;

  memset(p_self, 0, sizeof(*p_self));

  //INFO: telemetry infos:
  //https://docs.google.com/document/d/1pF7Y0WH9QRMogssxoEnF_wt2toIbfev2oNefXbzwYsw/edit?tab=t.0
  IpAddrIpV4Port addr =
  {
    .port = 8814,
#ifdef LOCAL_TELEMETRY
    .addr = "127.0.0.1",
  };
#pragma message "using local telemetry"
#else
    .addr = "204.216.214.158",
  };
#endif //!LOCAL_TELEMTRY
  
  if(hardware_ethernet_udp_init(&p_self->p_ethernet_udp_telemetry,&addr)<0)
  {
    goto udp_hardware_failed;
  }


  return 0;

  err--;
  hardware_ethernet_udp_free(&p_self->p_ethernet_udp_telemetry);
udp_hardware_failed:
  err--;

  return err;
}

int8_t log_telemetry_add_entry(LogTelemetry_h* const restrict self __attribute__((__unused__)),
    const char* name, const DataFormat data_format, const void* const var, const enum DATA_MODE data_type)
{
  JsonCell* json_cell = NULL;

  if( (json_cell = _search_json_for_log_var(name)) == NULL)
  {
    serial_write_raw("log var not found: ");
    serial_write_str(name);
    SET_TRACE(CORE_1, INIT_ERROR);
    return -1;
  }

  if (json_cell->p_data)
  {
    serial_write_raw("warning updating var in use:");
    serial_write_raw(json_cell->m_name);
    serial_write_str("");
  }


  json_cell->m_data_type = data_type;
  json_cell->p_data = var;
  json_cell->data_format = data_format;

  return 0;
}

int8_t log_telemetry_send(LogTelemetry_h* const restrict self)
{
  union LogTelemetry_h_t_conv conv = {self};
  struct LogTelemetry_t* const restrict p_self = conv.clear;
  JsonCell* p_log_page = NULL;
  Json_h json = {0};
  int8_t err=0;
  const uint8_t MAX_RETRY_INIT_JSON = 255;

  p_self->m_timestamp = timer_time_now();

  for (uint8_t i=0; i<JSON_ARRAY_COUNT; i++)
  {
    uint8_t retry=0;
    while(json_init(&json)<0 && retry < MAX_RETRY_INIT_JSON)
    {
      ++retry;
    }
    if (retry == MAX_RETRY_INIT_JSON)
    {
      SET_TRACE(CORE_1, INIT_ERROR);
      continue;
    }

    p_log_page = JSON_ARRAYS[i];
    for (uint8_t j=0;j<JSON_ARRAY_SIZES[i];j++)
    {
      JsonCell* cursor = &p_log_page[j];
      if (cursor->p_data)
      {
        uint8_t size=0;
        switch (cursor->m_data_type)
        {
          case __i8__:
          case __u8__:
            size=1;
            break;
          case __i16__:
          case __u16__:
            size=2;
            break;
          case __i32__:
          case __u32__:
          case __float__:
            size=4;
            break;
          default:
            break;
        }
        (void)size;
        json_push_element(&json, cursor->m_name,size, cursor->data_format, cursor->p_data);
      }
    }
    UdpIpv4Mex mex = {
      .data_length = (uint16_t) json_len(&json),
      .raw_data = json_get(&json),
    };
    hardware_ethernet_udp_send(&p_self->p_ethernet_udp_telemetry, &mex);
    json_destroy(&json);
    p_log_page = NULL;
  }

  return err;
}


int8_t log_telemetry_destroy(LogTelemetry_h* const restrict self)
{
  union LogTelemetry_h_t_conv conv = {self};
  struct LogTelemetry_t* const restrict p_self = conv.clear;

  hardware_ethernet_udp_free(&p_self->p_ethernet_udp_telemetry);

  memset(self, 0, sizeof(*self));

  return 0;
}
