#ifndef RACEUP_COMPONENTS_COMPAT_H
#define RACEUP_COMPONENTS_COMPAT_H

#ifdef __cplusplus
#include <atomic>
#ifndef restrict
#define restrict __restrict
#endif
#define RACEUP_ALIGNOF(T) alignof(T)
using std::atomic_bool;
using std::atomic_flag;
#ifndef ATOMIC_FLAG_INIT
#define ATOMIC_FLAG_INIT std::atomic_flag()
#endif
#ifndef atomic_flag_test_and_set
#define atomic_flag_test_and_set(obj) ((obj)->test_and_set(std::memory_order_seq_cst))
#endif
#ifndef atomic_flag_clear
#define atomic_flag_clear(obj) ((obj)->clear(std::memory_order_seq_cst))
#endif
#ifndef atomic_load
#define atomic_load(obj) ((obj)->load(std::memory_order_seq_cst))
#endif
#ifndef atomic_store
#define atomic_store(obj, val) ((obj)->store((val), std::memory_order_seq_cst))
#endif
#ifndef ATOMIC_VAR_INIT
#define ATOMIC_VAR_INIT(value) (value)
#endif
#else
#define RACEUP_ALIGNOF(T) _Alignof(T)
#endif

#endif
