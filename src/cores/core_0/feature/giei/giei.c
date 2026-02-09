#include "giei.h"
#include "../../../core_utility/core_utility.h"
#include "../../../../lib/raceup_board/raceup_board.h"
#include "../driver_input/driver_input.h"
#include "../maps/maps.h"
#include "../../../core_utility/imu/imu.h"
#include "giei_components/giei_components.h"
#include "giei_components/hv/hv.h"
#include "../math_saturated/saturated.h"

#include <stdint.h>
#include <string.h>

//private

#define M_N                                 9.8f

struct Giei_t{
    Hv_h hv;
    time_var_microseconds rtd_sound_start;
    time_var_microseconds m_last_sent_rpm;
    time_var_microseconds m_last_bms_estimate;
    RtdAssiSound_h m_rtd_sound;
    GpioRead_h gpio_rtd_button;
    Gpio_h m_gpio_rtd_button_led;
    MissionLocker_h m_mission_locker;
    GlobalRunningStatusMut_h m_global_running_status_owner;
    float engines_voltages[__NUM_OF_ENGINES__];
    enum RUNNING_STATUS running_status;
    EngineType* inverter;
    const DriverInput_h* driver_input;
    const DrivingMaps_h* driving_maps;
    const Imu_h* imu;
    struct CanMailbox* p_send_dv_rpm;
    uint32_t entered_rtd;
    EmergencyNode_h m_emergency_node;
    uint8_t m_emergency_cs;
};

union Giei_conv{
    Giei_h* const hidden;
    struct Giei_t* const clear;
};

union Giei_conv_const{
    const Giei_h* const hidden;
    const struct Giei_t* const clear;
};

#define GIEI_H_T_CONV(h_ptr, t_ptr_name)\
    union Giei_conv __g_conv_##t_ptr_name##__ = {h_ptr};\
    struct Giei_t* t_ptr_name = __g_conv_##t_ptr_name##__.clear;

#ifdef DEBUG
char __giei_size_check[(sizeof(Giei_h) == sizeof(struct Giei_t))? 1 : -1];
char __giei_align_check[(_Alignof(Giei_h) == _Alignof(struct Giei_t))? 1 : -1];
#endif /* ifdef DEBUG */

//public

int8_t giei_init(Giei_h* const restrict self,
        EngineType* const engine,
        const DriverInput_h* const p_driver,
        const DrivingMaps_h* const p_maps,
        const Imu_h* const p_imu)
{
    GIEI_H_T_CONV(self, p_self);
    struct CanNode* p_node = NULL;
    int8_t err=0;

    while (EmergencyNode_init(&p_self->m_emergency_node)<0)
    {
      serial_write_str("giei EmergencyNode init");
      timer_wait(get_tick_from_millis(100));
    }

    if(hv_init(&p_self->hv)<0)
    {
      goto hv_failed;
    }

    if(hardware_init_read_permission_gpio(&p_self->gpio_rtd_button, GPIO_RTD_BUTTON)<0)
    {
      goto rtd_failed;
    }

    if(hardware_init_gpio(&p_self->m_gpio_rtd_button_led, GPIO_RTD_BUTTON_LED, 0)<0)
    {
      goto rtd_button_led_failed;
    }

    AssiMetadata data =
    {
      .duration = get_tick_from_millis(3000),
      .duty_cycle_min = 50,
      .duty_cycle_max = 100,
      .duty_cycle_step = get_tick_from_millis(300),
    };
    if(NS_ASSIS_SOUND(init)(&p_self->m_rtd_sound,&data)<0)
    {
      goto assi_sound_failed;
    }

    if (lock_mission_ref_get_mut(&p_self->m_mission_locker)<0)
    {
      goto lock_mission_ref_failed;
    }

    tv_alg_init();

    p_self->inverter = engine;
    p_self->driver_input = p_driver;
    p_self->driving_maps = p_maps;
    p_self->imu = p_imu;

    //INFO: sharing var:
    //  - posTorquesNM
    //  - negTorquesNM.
    //  - engines_voltages
    //
    //ranges min, max included
    //pos: 0..3
    //neg: 4..7
    //engines engines_voltages: 8..11

    // for (uint8_t i=0; i<__NUM_OF_ENGINES__; i++)
    // {
    //   if(external_log_variables_store_pointer(p_self->engines_voltages, (2 * __NUM_OF_ENGINES__) + i))return -7;
    // }

    ACTION_ON_CAN_NODE(CAN_DV, p_node)
    {
      p_self->p_send_dv_rpm =
        hardware_get_mailbox_single_mex(
            p_node,
            SEND_MAILBOX,
            CAN_ID_DV_RPM,
            (uint16_t) message_dlc_can3(CAN_ID_DV_RPM));
    }

    if (!p_self->p_send_dv_rpm)
    {
      goto mailbox_send_dv_rpm_failed;
    }

    if (global_running_status_mut_init(&p_self->m_global_running_status_owner)<0)
    {
      goto globla_running_status_mut_set_init_failed;
    }

    return 0;



    err--;
globla_running_status_mut_set_init_failed:
    err--;
    hardware_free_mailbox_can(&p_self->p_send_dv_rpm);
mailbox_send_dv_rpm_failed:
    err--;
    lock_mission_ref_destroy_mut(&p_self->m_mission_locker);
lock_mission_ref_failed:
    err--;
    NS_ASSIS_SOUND(destroy)(&p_self->m_rtd_sound);
assi_sound_failed:
    err--;
    gpio_destroy(&p_self->m_gpio_rtd_button_led);
rtd_button_led_failed:
    err--;
    gpio_read_destroy(&p_self->gpio_rtd_button);
rtd_failed:
    err--;
    hv_destroy(&p_self->hv);
hv_failed:
    err--;

    return err;
}

int8_t giei_update(Giei_h* const restrict self)
{
  uint64_t rpm_payload=0;
  can_obj_can3_h_t o3 = {0};
  int8_t err=0;
  GIEI_H_T_CONV(self, p_self);

  if (hv_update(&p_self->hv)<0)
  {
    SET_TRACE(CORE_0, UPDATE_ERR);
    err--;
  }

  if(inverter_update(p_self->inverter)<0)
  {
    SET_TRACE(CORE_0, UPDATE_ERR);
    err--;
  }


  ACTION_ON_FREQUENCY(p_self->m_last_sent_rpm, get_tick_from_millis(5))
  {
    uint16_t data_fl = (uint16_t) engine_get_info(p_self->inverter,FRONT_LEFT,ENGINE_RPM);
    uint16_t data_fr = (uint16_t) engine_get_info(p_self->inverter,FRONT_RIGHT,ENGINE_RPM);
    uint16_t data_rl = (uint16_t) engine_get_info(p_self->inverter,REAR_LEFT,ENGINE_RPM);
    uint16_t data_rr = (uint16_t) engine_get_info(p_self->inverter,REAR_RIGHT,ENGINE_RPM);

    o3.can_0x0c8_DV_Rpm.rpm_fl = data_fl;
    o3.can_0x0c8_DV_Rpm.rpm_fr = data_fr;
    o3.can_0x0c8_DV_Rpm.rpm_rl = data_rl;
    o3.can_0x0c8_DV_Rpm.rpm_rr = data_rr;

    if(pack_message_can3(&o3, CAN_ID_DV_RPM, &rpm_payload)<0)
    {
      SET_TRACE(CORE_0, PACK_ERROR);
      err--;
    }

    if (hardware_mailbox_send(p_self->p_send_dv_rpm, rpm_payload)<0)
    {
      SET_TRACE(CORE_0, CAN_SEND);
      err--;
    }
  }

  ACTION_ON_FREQUENCY(p_self->m_last_bms_estimate, get_tick_from_millis(20))
  {
    if(hv_computeBatteryPackTension(&p_self->hv,p_self->engines_voltages,__NUM_OF_ENGINES__)<0)
    {
      SET_TRACE(CORE_0, UPDATE_ERR);
      err--;
    }
  }

  tv_alg_update();

  return err;
}

enum RUNNING_STATUS GIEI_check_running_condition(Giei_h* const restrict self)
{
    GIEI_H_T_CONV(self, p_self);
    enum RUNNING_STATUS rt = SYSTEM_OFF;

    if (as_node_read_get())
    {
      rt = engine_rtd_procedure(p_self->inverter);
      if(global_running_status_set(&p_self->m_global_running_status_owner, rt)<0)
      {
        SET_TRACE(CORE_0, UPDATE_ERR);
      }
    }
    else
    {
      EmergencyNode_solve(&p_self->m_emergency_node, 0);
      engine_reset(p_self->inverter);
    }


    if (rt > SYSTEM_OFF)
    {
      lock_mission(&p_self->m_mission_locker);
    }
    else
    {
      unlock_mission(&p_self->m_mission_locker);
    }
    if (rt == RUNNING && !p_self->entered_rtd)
    {
        p_self->entered_rtd =1;
        gpio_set_high(&p_self->m_gpio_rtd_button_led);
        NS_ASSIS_SOUND(start)(&p_self->m_rtd_sound);
        p_self->rtd_sound_start = timer_time_now();
    }
    else if (rt != RUNNING)
    {
        p_self->entered_rtd =0;
        p_self->rtd_sound_start=0;
        gpio_set_low(&p_self->m_gpio_rtd_button_led);
        NS_ASSIS_SOUND(stop)(&p_self->m_rtd_sound);
    }
    p_self->running_status = rt;
    return rt;

}

int8_t GIEI_compute_power(Giei_h* const restrict self)
{
    const union Giei_conv conv = {self};
    struct Giei_t* const restrict p_self = conv.clear;

    float TorquesNM[__NUM_OF_ENGINES__] = {0};

    const float driver_throttle = giei_driver_input_get(p_self->driver_input, THROTTLE);
    const float driver_brake = giei_driver_input_get(p_self->driver_input, BRAKE);
    const float power_repartition = driving_map_get_parameter(p_self->driving_maps, TORQUE_REPARTITION);
    const float giei_max_pos_torque = driving_map_get_parameter(p_self->driving_maps, MAX_POS_TORQUE);
    const float giei_max_neg_torque = driving_map_get_parameter(p_self->driving_maps, MAX_NEG_TORQUE);
    const float giei_max_regen_current = driving_map_get_parameter(p_self->driving_maps, REGEN_SCALE);

    p_self->engines_voltages[FRONT_LEFT] = engine_get_info(p_self->inverter, FRONT_LEFT,ENGINE_VOLTAGE);
    p_self->engines_voltages[FRONT_RIGHT] = engine_get_info(p_self->inverter, FRONT_RIGHT,ENGINE_VOLTAGE);
    p_self->engines_voltages[REAR_LEFT] = engine_get_info(p_self->inverter, REAR_LEFT,ENGINE_VOLTAGE);
    p_self->engines_voltages[REAR_RIGHT] = engine_get_info(p_self->inverter, REAR_RIGHT,ENGINE_VOLTAGE);

    const struct TVInputArgs tv_input =
    {
      .acc_x = imu_get_acc(p_self->imu, AXES_X),
      .acc_y = imu_get_acc(p_self->imu, AXES_Y),
      .yaw_rate= -imu_get_omega(p_self->imu, AXES_Z),
      .throttle = driver_throttle,
      .brake = driver_brake,
      .steering_angle = giei_driver_input_get(p_self->driver_input, STEERING_ANGLE),
      .current = hv_get_info(&p_self->hv, HV_CURRENT_AMPERE),
      .voltage = hv_get_info(&p_self->hv, HV_BATTERY_PACK_TENSION),
      .limit_power = driving_map_get_parameter(p_self->driving_maps, POWER_KW),
      .torque_max = giei_max_pos_torque,
      .torque_regen_max = giei_max_neg_torque,
      .power_repartition = power_repartition,
      .regen_max_current = giei_max_regen_current,
      .Vcell_min = hv_get_info(&p_self->hv, HV_MIN_M_VOLT)/1000.0f,
      .TV_active = driving_map_get_parameter(p_self->driving_maps, TV_ON)>0,
      .TC_strat = (uint8_t) driving_map_get_parameter(p_self->driving_maps, TC_STRAT),
      .REGEN_active = !(giei_max_regen_current==0 && giei_max_neg_torque==0),
      .rpm[FRONT_LEFT] = engine_get_info(p_self->inverter, FRONT_LEFT, ENGINE_RPM),
      .rpm[FRONT_RIGHT] = engine_get_info(p_self->inverter, FRONT_RIGHT, ENGINE_RPM),
      .rpm[REAR_LEFT] = engine_get_info(p_self->inverter, REAR_LEFT, ENGINE_RPM),
      .rpm[REAR_RIGHT] = engine_get_info(p_self->inverter, REAR_RIGHT, ENGINE_RPM),
    };
    tv_alg_compute(&tv_input, TorquesNM);

    for (uint8_t i = 0; i < __NUM_OF_ENGINES__; i++)
    {
      if (TorquesNM[i] > 0 && driver_throttle==0.0f)
      {
        p_self->m_emergency_cs++;
        if (p_self->m_emergency_cs > 120) {
          SET_TRACE(CORE_0, UNREACHABLE_CODE);
          EmergencyNode_raise(&p_self->m_emergency_node, 0);
        }
      }else if (TorquesNM[i]<0)
      {
        if (car_speed_get()<0.5f)
        {
          engine_send_torque(p_self->inverter, i, 0, 0);
        }
        else
        {
          engine_send_torque(p_self->inverter, i, 0, TorquesNM[i]);
        }
        EmergencyNode_solve(&p_self->m_emergency_node, 0);
        p_self->m_emergency_cs =0;
      }
      else
      {
        engine_send_torque(p_self->inverter, i, TorquesNM[i], 0);
        EmergencyNode_solve(&p_self->m_emergency_node, 0);
        p_self->m_emergency_cs =0;
      }
    }

    return 0;
}

int8_t giei_destroy(Giei_h* const restrict self)
{
  GIEI_H_T_CONV(self, p_self);

  lock_mission_ref_destroy_mut(&p_self->m_mission_locker);
  NS_ASSIS_SOUND(destroy)(&p_self->m_rtd_sound);
  gpio_destroy(&p_self->m_gpio_rtd_button_led);
  gpio_read_destroy(&p_self->gpio_rtd_button);
  hv_destroy(&p_self->hv);

  memset(self, 0, sizeof(*self));

  return 0;
}
