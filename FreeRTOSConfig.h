#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <stdint.h>
#include <tricoreintrin.h>

#ifndef __disable
#define __disable() __builtin_tricore_disable()
#endif
#ifndef __enable
#define __enable() __builtin_tricore_enable()
#endif
#ifndef __mfcr
#define __mfcr(reg) __builtin_tricore_mfcr((reg))
#endif
#ifndef __mtcr
#define __mtcr(reg, val) __builtin_tricore_mtcr((reg), (val))
#endif
#ifndef __dsync
#define __dsync() __builtin_tricore_dsync()
#endif
#ifndef __isync
#define __isync() __builtin_tricore_isync()
#endif
#ifndef __nop
#define __nop() __builtin_tricore_nop()
#endif
#ifndef __syscall
#define __syscall(id) __builtin_tricore_syscall((unsigned)(id))
#endif
#ifndef __clz
#define __clz(x) __builtin_clz((unsigned)(x))
#endif
#ifndef __extru
#define __extru(val, pos, width) __builtin_tricore_extr_u((unsigned)(val), (unsigned)(pos), (unsigned)(width))
#endif
#ifndef __insert
#define __insert(src, val, pos, width) __builtin_tricore_insert((unsigned)(src), (unsigned)(val), (unsigned)(pos), (unsigned)(width))
#endif
#ifndef __vector_table
#define __vector_table(cpu)
#endif
#ifndef __interrupt
#define __interrupt(prio) __attribute__((interrupt))
#endif

#define configUSE_PREEMPTION 1
#define configUSE_IDLE_HOOK 0
#define configUSE_TICK_HOOK 0
#define configCPU_CLOCK_HZ (300000000UL)
#define configTICK_RATE_HZ (1000U)
#define configMAX_PRIORITIES (5U)
#define configMINIMAL_STACK_SIZE (256U)
#define configTOTAL_HEAP_SIZE (16U * 1024U)
#define configMAX_TASK_NAME_LEN (16U)
#define configUSE_16_BIT_TICKS 0
#define configIDLE_SHOULD_YIELD 1
#define configUSE_MUTEXES 0
#define configCHECK_FOR_STACK_OVERFLOW 0
#define configUSE_RECURSIVE_MUTEXES 0
#define configUSE_MALLOC_FAILED_HOOK 0
#define configUSE_APPLICATION_TASK_TAG 0
#define configUSE_COUNTING_SEMAPHORES 0
#define configUSE_TIME_SLICING 1

#define configSUPPORT_STATIC_ALLOCATION 0
#define configSUPPORT_DYNAMIC_ALLOCATION 1

#define configUSE_CO_ROUTINES 0
#define configUSE_TIMERS 0

#define INCLUDE_vTaskDelay 1
#define INCLUDE_vTaskDelayUntil 1

#define configSTM_CLOCK_HZ (100000000UL)
#define configCPU_NR 0
#define configTIMER_INTERRUPT_PRIORITY 10
#define configCONTEXT_INTERRUPT_PRIORITY 11
#define configMAX_API_CALL_INTERRUPT_PRIORITY 12
#define configSYSCALL_CALL_DEPTH 1
#define configPROVIDE_SYSCALL_TRAP 0
#define configCPU_STM_DEBUG 0
#define configTICK_STM_DEBUG 0

#define configSTM ((volatile uint32_t *)0xF0001000u)
#define configSTM_SRC ((volatile uint32_t *)0xF0038300u)
#define configCONTEXT_SRC ((volatile uint32_t *)0xF0038000u)

#define configASSERT(x) if ((x) == 0) { taskDISABLE_INTERRUPTS(); for(;;) {;} }

#endif /* FREERTOS_CONFIG_H */
