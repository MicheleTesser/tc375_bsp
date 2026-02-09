#ifndef __LOG__
#define __LOG__

#include <stdint.h>

#include "./log_obj_types.h"

#define LOG_ENTRY_NAME_MAX_SIZE 64

enum LOG_MODE{
  LOG_SD,
  LOG_TELEMETRY,

  __NUM_OF_LOG_MODES__
};

typedef struct{
  uint8_t log_mode:2;
  enum DATA_MODE data_mode;
  const void* data_ptr;
  DataFormat data_format;
  char name[LOG_ENTRY_NAME_MAX_SIZE];
}LogEntry_h;

typedef int8_t (*log_add_enty_f)(void* const log, const LogEntry_h* const);

__attribute__((__nonnull__(1)))
int8_t log_set_log_ptr(void* const log, const log_add_enty_f add_entry_f);

int8_t log_unset_log_ptr(void);

int8_t log_ptr_setted(void);

__attribute__((__nonnull__(1)))
int8_t log_add_entry(const LogEntry_h* entry);

#endif // !__LOG__
