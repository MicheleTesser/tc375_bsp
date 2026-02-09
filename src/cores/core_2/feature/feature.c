#include "feature.h"

#include "DV/dv.h"
#include "../../../lib/raceup_board/raceup_board.h"
#include "../../core_utility/core_utility.h"

#include <stdint.h>

struct Core2Feature_t{
  CarMissionReader_h m_mission;
  Dv_h m_dv;
};

union Core2Feature_h_t_conv{
  Core2Feature_h* const restrict hidden;
  struct Core2Feature_t* const restrict clear;
};

#ifdef DEBUG
char __assert_size_core_2_feature[(sizeof(Core2Feature_h) == sizeof(struct Core2Feature_t))? 1:-1];
char __assert_align_core_2_feature[(_Alignof(Core2Feature_h) == _Alignof(struct Core2Feature_t))? 1:-1];
#endif // DEBUG

int8_t core_2_feature_init(Core2Feature_h* const restrict self )
{
  union Core2Feature_h_t_conv conv = {self};
  struct Core2Feature_t* const restrict p_self = conv.clear;
  int8_t err=0;

  if(NS_CAR_MISS(init)(&p_self->m_mission)<0)
  {
    goto car_mission_failed;
  }

  if(NS_DV(init)(&p_self->m_dv, &p_self->m_mission) <0)
  {
    goto dv_failed;
  }

  return 0;

  err--;
  NS_DV(destroy)(&p_self->m_dv);
dv_failed:
  err--;
  NS_CAR_MISS(destroy)(&p_self->m_mission);
car_mission_failed:
  err--;

  return err;
}

#define ERR_TRACE(ERR)\
  {\
    SET_TRACE(CORE_2,ERR);\
    err--;\
  }

int8_t core_2_feature_update(Core2Feature_h* const restrict self )
{
  union Core2Feature_h_t_conv conv = {self};
  struct Core2Feature_t* const restrict p_self = conv.clear;
  int8_t err=0;

  if(NS_CAR_MISS(update)(&p_self->m_mission) <0) ERR_TRACE(UPDATE_ERR);
  if(NS_DV(update)(&p_self->m_dv) <0) ERR_TRACE(UPDATE_ERR);

  return err;
}
