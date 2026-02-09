#ifndef __CAR_MISSION_READER__
#define __CAR_MISSION_READER__

#include <stdint.h>
#include "mission_locker/mission_locker.h"

#define NS_CAR_MISS(name) CAR_MISSION_READER_##name

typedef struct __attribute__((aligned(4))) CarMissionReader_h{
  const uint8_t private_data[38];
  MissionLockerRead_h o_mission_locker_read;
}CarMissionReader_h;

/*
FROM can2.dbc

0 "none";
1 "manualy" 
2 "dv_acceleration" 
3 "dv_skidpad" 
4 "dv_autocross" 
5 "dv_trackdrive" 
6 "dv_ebs_test" 
7 "dv_inspection" 
*/

enum NS_CAR_MISS(MISSION)
{
  NS_CAR_MISS(NONE)=0,
  NS_CAR_MISS(HUMAN),
  NS_CAR_MISS(DV_ACCELERATION),
  NS_CAR_MISS(DV_SKIDPAD),
  NS_CAR_MISS(DV_AUTOCROSS),
  NS_CAR_MISS(DV_TRACKDRIVE),
  NS_CAR_MISS(DV_EBS_TEST),
  NS_CAR_MISS(DV_INSPECTION),

  NS_CAR_MISS(__NUM_OF__)
};

int8_t
NS_CAR_MISS(init)(CarMissionReader_h* const restrict self)__attribute__((__nonnull__(1)));

int8_t
NS_CAR_MISS(update)(CarMissionReader_h* const restrict self)__attribute__((__nonnull__(1)));

enum NS_CAR_MISS(MISSION)
NS_CAR_MISS(get_current_mission)(CarMissionReader_h* const restrict self)__attribute__((__nonnull__(1)));

int8_t
NS_CAR_MISS(destroy)(CarMissionReader_h* const restrict self)__attribute__((__nonnull__(1)));

#endif // !__CAR_MISSION_READER__
