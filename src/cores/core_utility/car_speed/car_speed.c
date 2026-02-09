#include "../../../lib/raceup_board/raceup_board.h"

#include "car_speed.h"
#include <stdatomic.h>
#include <stdint.h>
#include <string.h>

#define WHEEL_RADIUS 0.2032f
#define PI 3.14f

typedef uint32_t SpeedType; //INFO: Km/h

struct CarSpeedMut_t{
  uint8_t owner;
};

union CarSpeedMut_h_t_conv
{
  CarSpeedMut_h* const restrict hidden;
  struct CarSpeedMut_t* const restrict clear;
};

#ifdef DEBUG
char __assert_size_car_speed_mut[sizeof(CarSpeedMut_h)==sizeof(struct CarSpeedMut_t)?+1:-1];
char __assert_align_car_speed_mut[_Alignof(CarSpeedMut_h)==_Alignof(struct CarSpeedMut_t)?+1:-1];
#endif /* ifdef DEBUG */

static struct{
  float value; 
  _Atomic uint8_t owned;
}SPEED;

//public

#define M_S //INFO: measure unit m/s
#define KM_H //INFO: measure unit km/h

int8_t car_speed_mut_init(CarSpeedMut_h* const restrict self)
{
  union CarSpeedMut_h_t_conv conv = {self};
  struct CarSpeedMut_t* p_self = conv.clear;
  if (!atomic_exchange(&SPEED.owned, 1U))
  {
    p_self->owner = 1;
    return 0;
  }
  return -1;
}

int8_t car_speed_mut_update(CarSpeedMut_h* const restrict self, const float engines_rpm[4])
{
  union CarSpeedMut_h_t_conv conv = {self};
  struct CarSpeedMut_t* p_self  = conv.clear;

  if (p_self->owner)
  {
    const float avg_rpm = (engines_rpm[0] + engines_rpm[1] + engines_rpm[2] + engines_rpm[3])/4;
    const float tau = 12.667f;
    float speed M_S =(((avg_rpm/30.0f) * PI)/tau)*WHEEL_RADIUS;
    speed KM_H = speed * 3.6f;
    SPEED.value = speed;
    return 0;
  }

  return -1;
}

float car_speed_mut_destroy(CarSpeedMut_h* const restrict self)
{
  union CarSpeedMut_h_t_conv conv = {self};
  struct CarSpeedMut_t* p_self  = conv.clear;

  if (p_self->owner)
  {
    atomic_store(&SPEED.owned, 0U);
    memset(self, 0, sizeof(*self));
    return 0;
  }

  return -1;
}

float car_speed_get(void)
{
  return SPEED.value;
}
