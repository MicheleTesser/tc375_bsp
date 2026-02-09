#include "log.h"
#include "sd/sd.h"
#include "telemetry/telemetry.h"
#include "can_log/can_log.h"
#include "../../../core_utility/core_utility.h"
#include <stdint.h>
#include <string.h>

struct Log_t{
  CanLog_h m_can;
  LogSd_h m_sd;
  LogTelemetry_h m_telemetry;
  time_var_microseconds m_last_sent_telemetry;
};

union Log_h_t_conv{
  Log_h* const restrict hidden;
  struct Log_t* const restrict clear;
};

#ifdef DEBUG
char __assert_size_log[(sizeof(Log_h) == sizeof(struct Log_t))? 1:-1];
char __assert_align_log[(_Alignof(Log_h) == _Alignof(struct Log_t))? 1:-1];
#endif // DEBUG

int8_t _log_add_entry_full(void* const restrict log, const LogEntry_h* entry)
{
  union Log_h_t_conv conv = {log};
  struct Log_t* p_self = conv.clear;
  int8_t err=0;

  while (!log_ptr_setted());

  if (entry->log_mode & LOG_TELEMETRY) {
    if(log_telemetry_add_entry(
          &p_self->m_telemetry,
          entry->name,
          entry->data_format,
          entry->data_ptr,
          entry->data_mode)<0)
    {
      err +=1;
    }
  }
  

  //TODO: SD

  return -err;
}

//public

int8_t log_init(Log_h* const restrict self )
{
  union Log_h_t_conv conv = {self};
  struct Log_t* const restrict p_self = conv.clear;
  memset(p_self, 0, sizeof(*p_self));
  int8_t err=0;

  log_set_log_ptr(self, _log_add_entry_full);

  while(log_sd_init(&p_self->m_sd)<0)
  {
    serial_write_str("init failed log sd ");
  }

  while(log_telemetry_init(&p_self->m_telemetry)<0)
  {
    serial_write_str("init failed log telemetry");
  }

  while((err=can_log_init(&p_self->m_can))<0)
  {
    serial_write_raw("init failed can log");
    serial_write_int8_t(err);
    serial_write_str("");
  }



  return 0;
}


int8_t log_update_and_send(Log_h* const restrict self)
{
  union Log_h_t_conv conv = {self};
  struct Log_t* const restrict p_self = conv.clear;
  int8_t err =0;

  hardware_ethernet_update();

  ACTION_ON_FREQUENCY(p_self->m_last_sent_telemetry, get_tick_from_millis(50)) //INFO: grafana limit
  {
    if(log_telemetry_send(&p_self->m_telemetry)<0)
    {
      err |= 1;
    }
  }
  if(can_log_update(&p_self->m_can)<0)
  {
    err |= 2;
  }
  //TODO: SD

  return -err;
}

int8_t
log_destroy(Log_h* const restrict self )
{
  union Log_h_t_conv conv = {self};
  struct Log_t* const restrict p_self = conv.clear;
  
  log_unset_log_ptr();

  can_log_destroy(&p_self->m_can);
  log_telemetry_destroy(&p_self->m_telemetry);
  log_sd_destroy(&p_self->m_sd);


  memset(self, 0, sizeof(*self));

  return 0;
}
