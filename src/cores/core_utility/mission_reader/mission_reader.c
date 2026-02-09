#include "mission_reader.h"
#include "mission_locker/mission_locker.h"
#include "../../../lib/raceup_board/raceup_board.h"
#include "../shared_message/shared_message.h"
#include <stdint.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include "../../../lib/board_dbc/dbc/out_lib/can2/can2.h"
#pragma GCC diagnostic pop 

#include <string.h>
#include <stdatomic.h>

struct CarMissionReader_t{
  _Atomic uint8_t m_current_mission;
  time_var_microseconds m_embeed_last_alive;
  SharedMessageReader_h m_recv_current_mission;
  SharedMessageReader_h m_recv_embedded_alive;
  MissionLockerRead_h m_mission_locker_read;
};

union CarMissionReader_h_t_conv{
  CarMissionReader_h* const hidden;
  struct CarMissionReader_t* const clear;
};

union CarMissionReader_h_t_conv_const{
  const CarMissionReader_h* const hidden;
  const struct CarMissionReader_t* const clear;
};


#ifdef DEBUG
char __assert_size_car_mission_reader[(sizeof(CarMissionReader_h)==sizeof(struct CarMissionReader_t))?+1:-1];
char __assert_alignment_car_mission_reader[(_Alignof(CarMissionReader_h)==_Alignof(struct CarMissionReader_t))?+1:-1];
#endif /* ifdef DEBUG */

//public

int8_t NS_CAR_MISS(init)(CarMissionReader_h* const restrict self)
{
  union CarMissionReader_h_t_conv conv = {self};
  struct CarMissionReader_t* const p_self = conv.clear;
  int8_t err=0;

  memset(p_self, 0, sizeof(*p_self));


  if (shared_message_reader_init(&p_self->m_recv_current_mission, SHARED_MEX_CARMISSION))
  {
    goto shared_message_reader_car_mission_failed;
  }

  if (shared_message_reader_init(&p_self->m_recv_embedded_alive, SHARED_MEX_DV_EMBEDDED_STATUS))
  {
    goto shared_message_reader_embedded_alive_check_failed;
  }

  if (lock_mission_ref_get(&p_self->m_mission_locker_read)<0)
  {
    goto locker_mission_get_failed;
  }

  atomic_store(&p_self->m_current_mission, NS_CAR_MISS(HUMAN));

  return 0;

  err--;
  lock_mission_ref_destroy(&p_self->m_mission_locker_read);
locker_mission_get_failed:
  err--;
  shared_message_read_destroy(&p_self->m_recv_embedded_alive);
shared_message_reader_embedded_alive_check_failed:
  err--;
  shared_message_read_destroy(&p_self->m_recv_current_mission);
shared_message_reader_car_mission_failed:
  err--;

  return err;
}

int8_t NS_CAR_MISS(update)(CarMissionReader_h* const restrict self)
{
  union CarMissionReader_h_t_conv conv = {self};
  struct CarMissionReader_t* const p_self = conv.clear;
  uint64_t mex = {0};
  can_obj_can2_h_t o2= {0};
  const uint8_t current_mission = atomic_load(&p_self->m_current_mission);

  if (shared_message_read(&p_self->m_recv_embedded_alive, &mex))
  {
    p_self->m_embeed_last_alive = timer_time_now(); 
  }

  if (current_mission <= NS_CAR_MISS(HUMAN))
  {
    p_self->m_embeed_last_alive = timer_time_now();
  }

  if ((timer_time_now() - p_self->m_embeed_last_alive) > get_tick_from_millis(500) && 
      current_mission > NS_CAR_MISS(HUMAN)&&
      !is_mission_locked(&p_self->m_mission_locker_read))
  {
    // p_self->m_current_mission = NS_CAR_MISS(NONE);//HACK:
  }

  if (!is_mission_locked(&p_self->m_mission_locker_read) &&
      shared_message_read_unpack_can2(&p_self->m_recv_current_mission, &o2)>0)
  {
    atomic_store(&p_self->m_current_mission, o2.can_0x047_CarMission.Mission);
  }

  return 0;
}

enum NS_CAR_MISS(MISSION) NS_CAR_MISS(get_current_mission)(CarMissionReader_h* const restrict self)
{
  union CarMissionReader_h_t_conv_const conv = {self};
  const struct CarMissionReader_t* const p_self = conv.clear;

  return (enum NS_CAR_MISS(MISSION))atomic_load(&p_self->m_current_mission);
}

int8_t
NS_CAR_MISS(destroy)(CarMissionReader_h* const restrict self)
{
  union CarMissionReader_h_t_conv conv = {self};
  struct CarMissionReader_t* const p_self = conv.clear;

  lock_mission_ref_destroy(&p_self->m_mission_locker_read);
  shared_message_read_destroy(&p_self->m_recv_embedded_alive);
  shared_message_read_destroy(&p_self->m_recv_current_mission);

  return 0;
}
