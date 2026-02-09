#include "mission_locker.h"
#include <stdatomic.h>
#include <stdint.h>
#include <string.h>


struct MissionLockerRead_t{
  uint8_t filler;
};

struct MissionLocker_t{
  uint8_t locked:1;
};


union MissionLocker_h_t_conv{
  MissionLocker_h* const restrict hidden;
  struct MissionLocker_t* const restrict clear;
};

union MissionLockerRead_h_t_conv{
  MissionLockerRead_h* const restrict hidden;
  struct MissionLockerRead_t* const restrict clear;
};

union MissionLockerRead_h_t_conv_const{
  const MissionLockerRead_h* const restrict hidden;
  const struct MissionLockerRead_t* const restrict clear;
};

#ifdef DEBUG
char __assert_size_mission_locker[sizeof(MissionLocker_h)==sizeof(struct MissionLocker_t)?1:-1];
char __assert_align_mission_locker[_Alignof(MissionLocker_h)==_Alignof(struct MissionLocker_t)?1:-1];

char __assert_size_mission_locker_read[sizeof(MissionLockerRead_h)==sizeof(struct MissionLockerRead_t)?1:-1];
char __assert_align_mission_locker_read[_Alignof(MissionLockerRead_h)==_Alignof(struct MissionLockerRead_t)?1:-1];
#endif /* ifdef DEBUG */

static struct{
  _Atomic uint8_t mutex;
  uint8_t mission_lock;
}MISSION_LOCKER;

static inline void _lock_mission(struct MissionLocker_t* const restrict self)
{
  if (!self->locked)
  {
    while (atomic_exchange(&MISSION_LOCKER.mutex, 1U));
    ++MISSION_LOCKER.mission_lock;
    atomic_store(&MISSION_LOCKER.mutex, 0U);
  }
  self->locked=1;
}

static inline void _unlock_mission(struct MissionLocker_t* const restrict self)
{
  if (self->locked)
  {
    while (atomic_exchange(&MISSION_LOCKER.mutex, 1U));
    --MISSION_LOCKER.mission_lock;
    atomic_store(&MISSION_LOCKER.mutex, 0U);
  }
  self->locked=0;
}

//public

int8_t lock_mission_ref_get(MissionLockerRead_h* const restrict self)
{
  union MissionLockerRead_h_t_conv conv = {self};
  struct MissionLockerRead_t* p_self = conv.clear;
  memset(p_self, 0, sizeof(*p_self));
  return 0;
}
int8_t lock_mission_ref_get_mut(MissionLocker_h* const restrict self)
{
  union MissionLocker_h_t_conv conv = {self};
  struct MissionLocker_t* const restrict p_self = conv.clear;
  memset(p_self, 0, sizeof(*p_self));
  return 0;
}

int8_t lock_mission_ref_destroy(MissionLockerRead_h* const restrict self)
{
  memset(self, 0, sizeof(*self));
  return 0;
}

int8_t lock_mission_ref_destroy_mut(MissionLocker_h* const restrict self)
{
  union MissionLocker_h_t_conv conv = {self};
  struct MissionLocker_t* const restrict p_self = conv.clear;

  _unlock_mission(p_self);
  memset(p_self, 0, sizeof(*p_self));

  return 0;
}

void lock_mission(MissionLocker_h* const restrict self)
{
  union MissionLocker_h_t_conv conv = {self};
  struct MissionLocker_t* const restrict p_self = conv.clear;
  _lock_mission(p_self);
}

void unlock_mission(MissionLocker_h* const restrict self)
{
  union MissionLocker_h_t_conv conv = {self};
  struct MissionLocker_t* const restrict p_self = conv.clear;
  _unlock_mission(p_self);
}

uint8_t is_mission_locked(const MissionLockerRead_h* const restrict self)
{
  union MissionLockerRead_h_t_conv_const conv = {self};
  const struct MissionLockerRead_t* p_self __attribute__((__unused__)) = conv.clear;
  return MISSION_LOCKER.mission_lock;
}
