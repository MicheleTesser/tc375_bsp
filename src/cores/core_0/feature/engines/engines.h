#ifndef __GIEI_ENGINES__
#define __GIEI_ENGINES__

#include "../../feature/driver_input/driver_input.h"
#include <stdint.h>

#include "../../../core_utility/running_status/running_status.h"

enum ENGINES {
    FRONT_LEFT = 0,
    FRONT_RIGHT = 1,
    REAR_LEFT = 2,
    REAR_RIGHT = 3,

    __NUM_OF_ENGINES__
};

#define FOR_EACH_ENGINE(engine)\
  for(enum ENGINES engine =0;engine<__NUM_OF_ENGINES__;engine++)

enum ENGINE_INFO{
    ENGINE_VOLTAGE=0,
    ENGINE_RPM,
    TARGET_VELOCITY,
};

struct Inverter;

typedef int8_t
(*inverter_update) (struct Inverter* const restrict self)__attribute__((__nonnull__(1)));

typedef enum RUNNING_STATUS
(*inverter_rtd_procedure)(const struct Inverter* const restrict self )__attribute__((__nonnull__(1)));

typedef float
(*inverter_get_info)(const struct Inverter* const restrict self ,
        const enum ENGINES engine, const enum ENGINE_INFO info)__attribute__((__nonnull__(1)));

typedef int8_t
(*inverter_send_torque)(struct Inverter* const restrict self ,
        const enum ENGINES engine, const float pos_torque, const float neg_torque)
    __attribute__((__nonnull__(1)));

typedef void
(*inverter_destroy)(struct Inverter* const restrict self )__attribute__((__nonnull__(1)));

typedef void
(*inverter_reset)(struct Inverter* const restrict self )__attribute__((__nonnull__(1)));

typedef struct EngineType{
    inverter_update update_f;
    inverter_rtd_procedure rtd_f;
    inverter_get_info get_info_f;
    inverter_send_torque send_torque_f;
    inverter_destroy destroy_f;
    inverter_reset reset_f;

    struct Inverter* data;
}EngineType;

#define inverter_update(self) self->update_f(self->data)
#define engine_rtd_procedure(self) self->rtd_f(self->data)
#define engine_get_info(self, engine, info) self->get_info_f(self->data, engine, info)
#define engine_send_torque(self, engine, pos_torque, neg_torque) \
    self->send_torque_f(self->data, engine, pos_torque, neg_torque)
#define engine_reset(self) self->reset_f(self->data);
#define engine_destroy(self) self->destroy_f(self->data);

#endif // !__GIEI_ENGINES__
