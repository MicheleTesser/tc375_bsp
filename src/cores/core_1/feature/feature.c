#include "feature.h"

#include "batteries/batteries.h"
#include "core_1_imu/core_1_imu.h"
#include "log/log.h"
#include "cooling/cooling.h"
#include "suspensions/suspensions.h"
#include "../../core_utility/core_utility.h"

#include <stdint.h>
#include <string.h>

struct Core1Feature_t{
    Log_h m_log;
    CarBatteries_h m_batteries;
    Core1Imu_h m_core_1_imu;
    Suspensions_h m_suspensions;
    Cooling_h m_cooling;
    SytemSettingOwner_h m_setting_update;
    SharedMessageOwner_h m_shared_message_owner;
};

union Core1Feature_h_t_conv{
    Core1Feature_h* const restrict hidden;
    struct Core1Feature_t* const restrict clear;
};

#ifdef DEBUG
char __assert_size_core_1_feature[(sizeof(Core1Feature_h) == sizeof(struct Core1Feature_t))? 1:-1];
char __assert_align_core_1_feature[(_Alignof(Core1Feature_h) == _Alignof(struct Core1Feature_t))? 1:-1];
#endif // DEBUG

int8_t core_1_feature_init(Core1Feature_h* const restrict self)
{
    union Core1Feature_h_t_conv conv = {self};
    struct Core1Feature_t* const restrict p_self = conv.clear;

    memset(p_self, 0, sizeof(*p_self));

    while(shared_message_owner_init(&p_self->m_shared_message_owner)< 0)
    {
      serial_write_str("core 1 failed owner messge");
    }
    while(system_settings_init(&p_self->m_setting_update)<0)
    {
      serial_write_str("core 1 failed system settings");
    }
    while(log_init(&p_self->m_log)<0)
    {
      serial_write_str("core 1 failed log");
    }
    while(car_batteries_init(&p_self->m_batteries) <0)
    {
      serial_write_str("core 1 failed batteries");
    }
    while(core_1_imu_init(&p_self->m_core_1_imu)<0)
    {
      serial_write_str("core 1 failed imu");
    }
    while(suspensions_init(&p_self->m_suspensions)<0)
    {
      serial_write_str("core 1 failed sups");
    }
    while(cooling_init(&p_self->m_cooling)<0)
    {
      serial_write_str("core 1 failed cooling");
    }

    return 0;
}

#define TRACE_ERR(ERR)\
{\
  SET_TRACE(CORE_1,ERR);\
  err--;\
}

int8_t core_1_feature_update(Core1Feature_h* const restrict self )
{
    union Core1Feature_h_t_conv conv = {self};
    struct Core1Feature_t* const restrict p_self = conv.clear;
    int8_t err=0;

    if(shared_message_owner_update(&p_self->m_shared_message_owner)<0) TRACE_ERR(UPDATE_ERR);
    if(car_batteries_update(&p_self->m_batteries)) TRACE_ERR(UPDATE_ERR);
    if(core_1_imu_update(&p_self->m_core_1_imu)) TRACE_ERR(UPDATE_ERR);
    if(suspensions_update(&p_self->m_suspensions)) TRACE_ERR(UPDATE_ERR);
    if(log_update_and_send(&p_self->m_log)<0)TRACE_ERR(UPDATE_ERR);
    if(system_settings_update(&p_self->m_setting_update)<0)TRACE_ERR(UPDATE_ERR);
    if(cooling_update(&p_self->m_cooling)<0)TRACE_ERR(UPDATE_ERR);
    NS_ASSIS_SOUND(update)();

    return err;
}
