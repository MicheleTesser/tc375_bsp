#include "log.h"

#include <stdatomic.h>
#include <stdint.h>
#include <stddef.h>

static struct{
  void* p_log;
  _Atomic uint8_t m_lock;
  log_add_enty_f add_entry_f;
}G_LOG;

int8_t log_set_log_ptr(void* const log, const log_add_enty_f add_entry_f)
{
  if (!atomic_exchange(&G_LOG.m_lock, 1U))
  {
    G_LOG.p_log = log;
    G_LOG.add_entry_f = add_entry_f;
    return 0;
  }

  return -1;
}

int8_t log_unset_log_ptr(void)
{
  atomic_store(&G_LOG.m_lock, 0U);
  G_LOG.p_log = NULL;
  return 0;
}

int8_t log_ptr_setted(void)
{
  return !!G_LOG.p_log;
}

int8_t log_add_entry(const LogEntry_h* entry)
{
  if (!log_ptr_setted())
  {
    return -1;
  }

  return G_LOG.add_entry_f(G_LOG.p_log,entry);
}
