#include "feature.h"
#include "driver_input/driver_input.h"
#include "engines/amk/amk.h"
#include "engines/engines.h"
#include "giei/giei.h"
#include "maps/maps.h"
#include "../../core_utility/core_utility.h"

#include <stdint.h>

struct Core0Feature_t{
  DriverInput_h m_driver;
  DrivingMaps_h maps;
  Imu_h imu;
  AmkInverter_h amk;
  EngineType inverter;
  Giei_h giei;
  CarMissionReader_h m_car_mission_reader;
  enum RUNNING_STATUS old_running_status;
  time_var_microseconds m_last_time_cs_compute;
  ControlFreq_h m_freq_compute_power;
};

union Core0Feature_h_t_conv{
  Core0Feature_h* const restrict hidden;
  struct Core0Feature_t* const restrict clear;
};

#ifdef DEBUG
char __assert_size_core_0_feature[(sizeof(Core0Feature_h)) == sizeof(struct Core0Feature_t)? 1:-1];
char __assert_align_core_0_feature[(_Alignof(Core0Feature_h)) == _Alignof(struct Core0Feature_t)? 1:-1];
#endif // DEBUG

//public

int8_t core_0_feature_init(Core0Feature_h* const restrict self )
{
  union Core0Feature_h_t_conv conv = {self};
  struct Core0Feature_t* const restrict p_self = conv.clear;
  int8_t err=0;

  if(NS_CAR_MISS(init)(&p_self->m_car_mission_reader))
  {
    goto car_mission_failed;
  }

  if(driver_input_init(&p_self->m_driver, &p_self->m_car_mission_reader) <0)
  {
    goto driver_input_failed;
  }
  if(driving_maps_init(&p_self->maps) <0)
  {
    goto map_failed;
  }
  if(imu_init(&p_self->imu) <0)
  {
    goto imu_failed;
  }
  if(amk_module_init(&p_self->amk, &p_self->m_driver, &p_self->inverter)<0)
  {
    goto engines_failed;
  }
  if(giei_init(&p_self->giei, &p_self->inverter, &p_self->m_driver, &p_self->maps,&p_self->imu) <0)
  {
    goto giei_failed;
  }

  while (control_freq_init(&p_self->m_freq_compute_power,
        "FREQ_compute_power_duration", "FREQ_compute_power_delay")<0)
  {
    serial_write_str("init failed feature 0 freq compute power");
    timer_wait(get_tick_from_millis(200));
  }

  return 0;

  err--;
  giei_destroy(&p_self->giei);
giei_failed:
  err--;
  p_self->inverter.destroy_f(p_self->inverter.data);
engines_failed:
  err--;
  imu_destroy(&p_self->imu);
imu_failed:
  err--;
  driving_maps_destroy(&p_self->maps);
map_failed:
  err--;
  driver_input_destroy(&p_self->m_driver);
driver_input_failed:
  err--;
  NS_CAR_MISS(destroy)(&p_self->m_car_mission_reader);
car_mission_failed:
  err--;

  return err;
}

#define ERR_TRACE()\
{\
  SET_TRACE(CORE_0, UPDATE_ERR);\
  err--;\
}

int8_t core_0_feature_update(Core0Feature_h* const restrict self )
{
  union Core0Feature_h_t_conv conv = {self};
  struct Core0Feature_t* const restrict p_self = conv.clear;
  int8_t err=0;

  if(NS_CAR_MISS(update)(&p_self->m_car_mission_reader))ERR_TRACE();
  if(giei_driver_input_update(&p_self->m_driver)<0) ERR_TRACE();
  if(driving_map_update(&p_self->maps) <0) ERR_TRACE();
  if(imu_update(&p_self->imu) <0) ERR_TRACE();
  if(giei_update(&p_self->giei)<0)ERR_TRACE();

  return err;
}


int8_t core_0_feature_compute_power(Core0Feature_h* const restrict self )
{
  union Core0Feature_h_t_conv conv = {self};
  struct Core0Feature_t* const restrict p_self = conv.clear;
  enum RUNNING_STATUS status = SYSTEM_OFF;
  int8_t err=0;

  status = GIEI_check_running_condition(&p_self->giei);

  if (status == RUNNING)
  {
    ACTION_ON_FREQUENCY(p_self->m_last_time_cs_compute, get_tick_from_millis(1))
    {
      MONITOR_FREQ_ACTION(p_self->m_freq_compute_power)
      {
        if(GIEI_compute_power(&p_self->giei)<0)
        {
          ERR_TRACE();
        }
      }
    }
  }

  return err;
}
