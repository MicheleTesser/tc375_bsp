#include <stdatomic.h>
#include <stdint.h>
#include <string.h>

#include "flash_memory.h"

struct FlashMemoryBank_t {
  BankId bank;
};

union FlashMemoryBank_h_t_conv {
  FlashMemoryBank_h *const restrict hidden;
  struct FlashMemoryBank_t *const restrict clear;
};

#ifdef DEBUG_BUILD
char __assert_size_flash
    [sizeof(FlashMemoryBank_h) == sizeof(struct FlashMemoryBank_t) ? 1 : -1];
char __assert_align_flash[_Alignof(FlashMemoryBank_h) ==
                                  _Alignof(struct FlashMemoryBank_t)
                              ? 1
                              : -1];
#endif /* ifdef DEBUG_BUILD */

#define FLASH_STORAGE_MAGIC 0x53464D31U /* "SFM1" */

typedef struct {
  uint32_t magic;
  uint32_t data[TOT_NUM_OF_BANKS][TOT_NUM_OF_PARTS];
  uint32_t checksum;
} FlashStorage;

__attribute__((section(".noinit"))) static FlashStorage g_flash_storage;

static struct {
  atomic_flag banks[TOT_NUM_OF_BANKS];
} BANK_STATUS;

#define LOCK_BANK(bank_id)                                                     \
  if (atomic_flag_test_and_set(&BANK_STATUS.banks[bank_id]))
#define UNLOCK_BANK(bank_id) atomic_flag_clear(&BANK_STATUS.banks[bank_id])

static uint32_t flash_calc_checksum(void) {
  uint32_t acc = 0U;
  for (uint32_t b = 0U; b < TOT_NUM_OF_BANKS; b++) {
    for (uint32_t p = 0U; p < TOT_NUM_OF_PARTS; p++) {
      acc ^= g_flash_storage.data[b][p];
      acc = (acc << 1U) | (acc >> 31U);
    }
  }
  return acc;
}

int8_t hardware_init_flash_memory(void) {
  for (uint32_t i = 0U; i < TOT_NUM_OF_BANKS; i++) {
    atomic_flag_clear(&BANK_STATUS.banks[i]);
  }

  if (g_flash_storage.magic != FLASH_STORAGE_MAGIC ||
      g_flash_storage.checksum != flash_calc_checksum()) {
    memset(&g_flash_storage, 0, sizeof(g_flash_storage));
    g_flash_storage.magic = FLASH_STORAGE_MAGIC;
    g_flash_storage.checksum = flash_calc_checksum();
  }

  return 0;
}

int8_t flash_memory_req_bank(FlashMemoryBank_h *const restrict self,
                             const BankId bank) {
  union FlashMemoryBank_h_t_conv conv = {self};
  struct FlashMemoryBank_t *const p_self = conv.clear;

  if (bank >= TOT_NUM_OF_BANKS) {
    return -1;
  }

  LOCK_BANK(bank) return -1;
  p_self->bank = bank;

  return 0;
}

int8_t flash_memory_read_bank(FlashMemoryBank_h *const restrict self,
                              uint32_t o_words[TOT_NUM_OF_PARTS]) {
  union FlashMemoryBank_h_t_conv conv = {self};
  struct FlashMemoryBank_t *const p_self = conv.clear;

  memcpy(o_words, g_flash_storage.data[p_self->bank],
         sizeof(*o_words) * TOT_NUM_OF_PARTS);

  return 0;
}

int8_t flash_memory_write_bank(FlashMemoryBank_h *const restrict self,
                               const uint32_t o_words[TOT_NUM_OF_PARTS]) {
  union FlashMemoryBank_h_t_conv conv = {self};
  struct FlashMemoryBank_t *const p_self = conv.clear;

  memcpy(g_flash_storage.data[p_self->bank], o_words,
         sizeof(*o_words) * TOT_NUM_OF_PARTS);
  g_flash_storage.checksum = flash_calc_checksum();

  return 0;
}

int8_t flash_memory_unleash_bank(FlashMemoryBank_h *const restrict self) {
  union FlashMemoryBank_h_t_conv conv = {self};
  struct FlashMemoryBank_t *const p_self = conv.clear;

  UNLOCK_BANK(p_self->bank);
  memset(self, 0, sizeof(*self));

  return 0;
}
