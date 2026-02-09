#ifndef __VIRTUAL_FLASH_MEMORY__
#define __VIRTUAL_FLASH_MEMORY__

#include <stdint.h>

#define TOT_NUM_OF_BANKS 32
#define TOT_NUM_OF_PARTS 2

typedef uint8_t BankId;
typedef struct __attribute__((aligned(1))){
  const uint8_t private_data[1];
}FlashMemoryBank_h;

extern int8_t hardware_init_flash_memory(void);

__attribute__((__nonnull__(1)))
extern int8_t flash_memory_req_bank(FlashMemoryBank_h* const restrict self, const BankId bank);

__attribute__((__nonnull__(1)))
extern int8_t flash_memory_read_bank(FlashMemoryBank_h* const restrict self, uint32_t o_words[2]);

__attribute__((__nonnull__(1)))
extern int8_t flash_memory_write_bank(FlashMemoryBank_h* const restrict self, const uint32_t o_words[2]);

__attribute__((__nonnull__(1)))
extern int8_t flash_memory_unleash_bank(FlashMemoryBank_h* const restrict self);

#endif // !__VIRTUAL_FLASH_MEMORY__
