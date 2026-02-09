#ifndef __RACEUP_BOARD_COMPONENT__
#define __RACEUP_BOARD_COMPONENT__ 

#include <stdint.h>

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
extern "C" {
#else
#include <stdatomic.h>
#define RACEUP_ALIGNOF(T) _Alignof(T)
#endif

#include "serial.h"
#include "gpio.h"
#include "can.h"
#include "timer.h"
#include "interrupt.h"
#include "trap.h"

#include "ethernet.h"
#include "adc.h"
#include "flash_memory.h"

#ifdef __cplusplus
}
#endif
#endif // !__RACEUP_BOARD_COMPONENT__
