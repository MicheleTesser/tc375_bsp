#include "component.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "IfxFlash.h"
#include "IfxCpu.h"
#include "Ifx_Types.h"


struct FlashMemoryBank_t{
  BankId bank;
};

union FlashMemoryBank_h_t_conv {
  FlashMemoryBank_h* const restrict hidden;
  struct FlashMemoryBank_t* const restrict clear;
};

#ifdef DEBUG
char __assert_size_flash[sizeof(FlashMemoryBank_h)==sizeof(struct FlashMemoryBank_t)?1:-1];
char __assert_align_flash[_Alignof(FlashMemoryBank_h)==_Alignof(struct FlashMemoryBank_t)?1:-1];
#endif /* ifdef DEBUG */

static struct{
  atomic_flag banks[TOT_NUM_OF_BANKS];
}BACNK_STATUS;


#define LOCK_BANK(bank_id) if (atomic_flag_test_and_set(&BACNK_STATUS.banks[bank_id]))
#define UNLOCK_BANK(bank_id) atomic_flag_clear(&BACNK_STATUS.banks[bank_id])

#define ACTION_ON_BANK(bank,err)\
  bank = _open_bank(p_self->bank);\
  if (bank<0) return err;\
  for (uint8_t bank_used=0;!bank_used;(close(bank), bank_used=1, bank=0))

#define ACTION_ON_CAN_NODE(node,p_node)\
  for (p_node =NULL;!p_node;p_node = hardware_init_can_get_ref_node(node));\
  for (uint8_t node_used=0;!node_used;(hardware_init_can_destroy_ref_node(&p_node), node_used=1, p_node=NULL))

#define DFLASH_PAGE_LENGTH          IFXFLASH_DFLASH_PAGE_LENGTH /* 0x8 = 8 Bytes (smallest unit that can be
                                                                 * programmed in the Data Flash memory (DFLASH))    */
#define FLASH_MODULE                0                           /* Macro to select the flash (PMU) module           */
#define PROGRAM_FLASH_0             IfxFlash_FlashType_P0       /* Define the Program Flash Bank to be used         */
#define DATA_FLASH_0                IfxFlash_FlashType_D0       /* Define the Data Flash Bank to be used            */

#define DATA_TO_WRITE               0x07738135                  /* Dummy data to be written into the Flash memories */

#define PFLASH_STARTING_ADDRESS     0xA00E0000                  /* Address of the PFLASH where the data is written  */
#define DFLASH_STARTING_ADDRESS     0xAF000000                  /* Address of the DFLASH where the data is written  */

#define DFLASH_NUM_PAGE_TO_FLASH    8                           /* Number of pages to flash in the DFLASH           */
#define DFLASH_NUM_SECTORS          1                           /* Number of DFLASH sectors to be erased            */

//public

int8_t hardware_init_flash_memory(void)
{
  return 0;
}

int8_t flash_memory_req_bank(FlashMemoryBank_h* const restrict self, const BankId bank)
{
  union FlashMemoryBank_h_t_conv conv = {self};
  struct FlashMemoryBank_t* const p_self = conv.clear;

  LOCK_BANK(bank) return -1;
  p_self->bank = bank;


  return 0;
}

int8_t flash_memory_read_bank(FlashMemoryBank_h* const restrict self, uint32_t o_words[TOT_NUM_OF_PARTS])
{
  union FlashMemoryBank_h_t_conv conv = {self};
  struct FlashMemoryBank_t* const p_self = conv.clear;

  const void* const pageAddr = (void*) (DFLASH_STARTING_ADDRESS + (p_self->bank * DFLASH_PAGE_LENGTH));    /* Get the address of the page  */

  memcpy(o_words, pageAddr, sizeof(*o_words) * TOT_NUM_OF_PARTS);

  return 0;
}

int8_t flash_memory_write_bank(FlashMemoryBank_h* const restrict self, const uint32_t o_words[TOT_NUM_OF_PARTS])
{
  union FlashMemoryBank_h_t_conv conv = {self};
  struct FlashMemoryBank_t* const p_self = conv.clear;
  uint64_t flash_copy[DFLASH_NUM_PAGE_TO_FLASH] = {0};
  uint32_t* cell = (uint32_t*) &flash_copy[p_self->bank];

  memcpy(flash_copy, (void*) DFLASH_STARTING_ADDRESS, sizeof(flash_copy[0]) * DFLASH_NUM_PAGE_TO_FLASH);
  cell[0] = o_words[0];
  cell[1] = o_words[1];

  /* --------------- ERASE PROCESS --------------- */
  /* Get the current password of the Safety WatchDog module */
  uint16 endInitSafetyPassword = IfxScuWdt_getSafetyWatchdogPassword();

  /* Erase the sector */
  IfxScuWdt_clearSafetyEndinit(endInitSafetyPassword);        /* Disable EndInit protection                       */
  IfxFlash_eraseMultipleSectors(DFLASH_STARTING_ADDRESS, DFLASH_NUM_SECTORS); /* Erase the given sector           */
  IfxScuWdt_setSafetyEndinit(endInitSafetyPassword);          /* Enable EndInit protection                        */

  /* Wait until the sector is erased */
  IfxFlash_waitUnbusy(FLASH_MODULE, DATA_FLASH_0);

  /* --------------- WRITE PROCESS --------------- */
  for(uint32_t page = 0; page < DFLASH_NUM_PAGE_TO_FLASH; page++)      /* Loop over all the pages                          */
  {
    uint32 pageAddr = DFLASH_STARTING_ADDRESS + (page * DFLASH_PAGE_LENGTH); /* Get the address of the page     */
    uint32_t* data_to_write = (uint32_t*) &flash_copy[page];

    /* Enter in page mode */
    IfxFlash_enterPageMode(pageAddr);

    /* Wait until page mode is entered */
    IfxFlash_waitUnbusy(FLASH_MODULE, DATA_FLASH_0);

    /* Load data to be written in the page */
    IfxFlash_loadPage2X32(pageAddr,data_to_write[0] , data_to_write[1] ); /* Load two words of 32 bits each            */

    /* Write the loaded page */
    IfxScuWdt_clearSafetyEndinit(endInitSafetyPassword);    /* Disable EndInit protection                       */
    IfxFlash_writePage(pageAddr);                           /* Write the page                                   */
    IfxScuWdt_setSafetyEndinit(endInitSafetyPassword);      /* Enable EndInit protection                        */

    /* Wait until the data is written in the Data Flash memory */
    IfxFlash_waitUnbusy(FLASH_MODULE, DATA_FLASH_0);
  }

  return 0;
}

int8_t flash_memory_unleash_bank(FlashMemoryBank_h* const restrict self)
{
  union FlashMemoryBank_h_t_conv conv = {self};
  struct FlashMemoryBank_t* const p_self = conv.clear;

  UNLOCK_BANK(p_self->bank);
  memset(self, 0, sizeof(*self));

  return 0;
}

