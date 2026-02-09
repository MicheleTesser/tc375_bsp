#include "core_1_imu.h"
#include "../log/log.h"
#include "../../../../lib/raceup_board/raceup_board.h"
#include "../../../core_utility/core_utility.h"

#include <stdint.h>
#include <string.h>

enum IMU_DATA{
  ACC_X=0,
  ACC_Y,
  ACC_Z,
  OMEGA_X,
  OMEGA_Y,
  OMEGA_Z,

  __NUM_OF_IMU_DATA__
};

struct Core1Imu_t{
  float imu_data[__NUM_OF_IMU_DATA__];
  uint32_t speed;
  Imu_h m_imu;
};

union Core1Imu_h_t_conv {
  Core1Imu_h* const restrict hidden;
  struct Core1Imu_t* const restrict clear;
};

//private

#ifdef DEBUG
uint8_t __assert_size_core_1_imu[(sizeof(Core1Imu_h) == sizeof(struct Core1Imu_t))?1:-1];
uint8_t __assert_align_core_1_imu[(_Alignof(Core1Imu_h) == _Alignof(struct Core1Imu_t))?1:-1];
#endif /* ifdef DEBUG */

#define IMU_LOG_VAR(mode,var_ptr, format, var_name, err_code)\
{\
  LogEntry_h log_entry={\
    .data_mode = mode,\
    .data_ptr = var_ptr,\
    .data_format= format,\
    .log_mode= LOG_SD | LOG_TELEMETRY,\
    .name = var_name,\
  };\
  if(log_add_entry(&log_entry)<0)\
  {\
    SET_TRACE(CORE_1, INIT_ERROR);\
    goto log_var_failed;\
  }\
}

int8_t core_1_imu_init(Core1Imu_h* const restrict self)
{
  union Core1Imu_h_t_conv conv = {self};
  struct Core1Imu_t* const restrict p_self = conv.clear;
  int8_t err=0;

  memset(p_self, 0, sizeof(*p_self));

  if (imu_init(&p_self->m_imu)<0)
  {
    goto imu_failed;
  }

  IMU_LOG_VAR(__float__,&p_self->imu_data[ACC_X], FLOAT, "acceleration_x",-1);
  IMU_LOG_VAR(__float__,&p_self->imu_data[ACC_Y], FLOAT, "acceleration_y",-2);
  IMU_LOG_VAR(__float__,&p_self->imu_data[ACC_Z], FLOAT, "acceleration_z",-3);
  IMU_LOG_VAR(__float__,&p_self->imu_data[OMEGA_X], FLOAT, "omega_x",-4);
  IMU_LOG_VAR(__float__,&p_self->imu_data[OMEGA_Y], FLOAT, "omega_y",-5);
  IMU_LOG_VAR(__float__,&p_self->imu_data[OMEGA_Z], FLOAT, "omega_z",-6);
  IMU_LOG_VAR(__u32__,&p_self->speed, UNSIGNED, "actual_velocity_kmh",-7);

  return 0;

log_var_failed:
  err--;
  imu_destroy(&p_self->m_imu);
imu_failed:
  err--;
  return err;
}

int8_t core_1_imu_update(Core1Imu_h* const restrict self)
{
  union Core1Imu_h_t_conv conv = {self};
  struct Core1Imu_t* const restrict p_self = conv.clear;

  if (imu_update(&p_self->m_imu)<0) SET_TRACE(CORE_1, UPDATE_ERR);

  p_self->imu_data[ACC_X] = imu_get_acc(&p_self->m_imu, AXES_X);
  p_self->imu_data[ACC_Y] = imu_get_acc(&p_self->m_imu, AXES_Y);
  p_self->imu_data[ACC_Z] = imu_get_acc(&p_self->m_imu, AXES_Z);
  p_self->imu_data[OMEGA_X] = imu_get_omega(&p_self->m_imu, AXES_X);
  p_self->imu_data[OMEGA_Y] = imu_get_omega(&p_self->m_imu, AXES_Y);
  p_self->imu_data[OMEGA_Z] = imu_get_omega(&p_self->m_imu, AXES_Z);
  p_self->speed = (uint32_t) car_speed_get();

  return 0;
}

int8_t core_1_imu_destroy(Core1Imu_h* const restrict self )
{
  union Core1Imu_h_t_conv conv = {self};
  struct Core1Imu_t* const restrict p_self = conv.clear;
  imu_destroy(&p_self->m_imu);

  memset(self, 0, sizeof(*self));

  return 0;
}
