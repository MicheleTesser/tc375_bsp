#include "can.h"
#include "bsp.h"

#include "uc/uc_tc37/TC37xPD_RegCHeaders/_Reg/IfxCan_reg.h"

#include <stdatomic.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#define CAN_RXSEL_A 0U
#define CAN_RXSEL_B 1U
#define CAN_RXSEL_C 2U
#define CAN_RXSEL_D 3U

#define CAN_TX_PC 2U

#define CAN_MAX_NODES 4U
#define CAN_RAM_WORDS_PER_NODE 0x200U

#define CAN_STD_FILTER_OFF_WORDS 0x40U
#define CAN_RX_FIFO0_OFF_WORDS 0x80U
#define CAN_TX_BUF_OFF_WORDS 0x100U

#define CAN_RX_FIFO0_SIZE 1U
#define CAN_TX_BUF_COUNT 1U

#define CAN_DATA_FIELD_8B 0U
#define CAN_MAX_MAILBOXES 8U

typedef struct {
  Ifx_CAN *module;
  uint8_t node_id;
  uint16_t ram_base_words;
  uint8_t rxsel;
  uint8_t tx_port;
  uint8_t tx_pin;
  uint8_t rx_port;
  uint8_t rx_pin;
  atomic_bool init_done;
  atomic_bool taken;
} CanNodeDriver;

typedef struct CanMailbox {
  CanNodeDriver *node;
  enum MAILBOX_TYPE type;
  uint16_t filter_id;
  uint16_t filter_mask;
  uint8_t mex_size;
  atomic_bool in_use;
} CanMailbox;

static CanNodeDriver g_nodes[__NUM_OF_CAN_MODULES__] = {0};
static CanMailbox g_mailboxes[CAN_MAX_MAILBOXES] = {0};

static Ifx_CAN *can_module_for_mod(enum CAN_MODULES mod, uint8_t *node_id) {
  switch (mod) {
  case CAN_INVERTER:
    *node_id = 1U;
    return &MODULE_CAN0;
  case CAN_GENERAL:
    *node_id = 2U;
    return &MODULE_CAN1;
  case CAN_DV:
    *node_id = 3U;
    return &MODULE_CAN0;
  case CAN_DEBUG:
    *node_id = 0U;
    return &MODULE_CAN0;
  default:
    *node_id = 0U;
    return NULL;
  }
}

static void can_config_pins(CanNodeDriver *node) {
  bsp_uc_port_EnableOutput(node->tx_port, node->tx_pin, CAN_TX_PC,
                           UC_PORT_PL_LEVEL_AL, UC_PORT_PD_SPEED_3);
  bsp_uc_port_EnableInput(node->rx_port, node->rx_pin,
                          UC_PORT_INPUT_MODE_TRISTATE);
}

static void can_config_module(CanNodeDriver *node, enum CAN_FREQUENCY baud) {
  Ifx_CAN *can = node->module;
  Ifx_CAN_N *n = &can->N[node->node_id];
  uint32_t brp = 9U;
  uint32_t tseg1 = 12U;
  uint32_t tseg2 = 3U;
  uint32_t sjw = 1U;
  uint32_t base_words = node->ram_base_words;
  uint32_t sidfc_words = base_words + CAN_STD_FILTER_OFF_WORDS;
  uint32_t rxf0_words = base_words + CAN_RX_FIFO0_OFF_WORDS;
  uint32_t txb_words = base_words + CAN_TX_BUF_OFF_WORDS;

  (void)baud;

  can->CLC.B.DISR = 0U;
  while (can->CLC.B.DISS != 0U) {
  }

  n->CCCR.B.INIT = 1U;
  n->CCCR.B.CCE = 1U;

  n->NPCR.B.RXSEL = node->rxsel;

  n->NBTP.B.NBRP = (uint8_t)brp;
  n->NBTP.B.NTSEG1 = (uint8_t)tseg1;
  n->NBTP.B.NTSEG2 = (uint8_t)tseg2;
  n->NBTP.B.NSJW = (uint8_t)sjw;

  n->GFC.B.ANFS = 2U;
  n->GFC.B.ANFE = 2U;
  n->GFC.B.RRFS = 1U;
  n->GFC.B.RRFE = 1U;

  n->SIDFC.B.FLSSA = (uint16_t)sidfc_words;
  n->SIDFC.B.LSS = 1U;

  n->RX.ESC.B.F0DS = CAN_DATA_FIELD_8B;
  n->RX.F0C.B.F0SA = (uint16_t)rxf0_words;
  n->RX.F0C.B.F0S = CAN_RX_FIFO0_SIZE;
  n->RX.F0C.B.F0WM = 0U;

  n->TX.ESC.B.TBDS = CAN_DATA_FIELD_8B;
  n->TX.BC.B.TBSA = (uint16_t)txb_words;
  n->TX.BC.B.NDTB = CAN_TX_BUF_COUNT;
  n->TX.BC.B.TFQS = 0U;
  n->TX.BC.B.TFQM = 0U;

  /* Standard filter: accept all IDs to FIFO0 */
  {
    volatile Ifx_CAN_STDMSG *filter =
        (volatile Ifx_CAN_STDMSG *)&can->RAM[sidfc_words];
    filter->S0.B.SFID1 = 0U;
    filter->S0.B.SFID2 = 0x7FFU;
    filter->S0.B.SFEC = 7U;
    filter->S0.B.SFT = 0U;
  }

  n->CCCR.B.INIT = 0U;
  n->CCCR.B.CCE = 0U;
}

int8_t hardware_init_can(const enum CAN_MODULES mod,
                         const enum CAN_FREQUENCY baud_rate) {
  CanNodeDriver *node;
  uint8_t node_id = 0U;
  Ifx_CAN *module = can_module_for_mod(mod, &node_id);
  uint16_t base_words;

  if (module == NULL) {
    return -1;
  }

  node = &g_nodes[mod];
  if (atomic_load(&node->init_done)) {
    return 0;
  }

  base_words = (uint16_t)(CAN_RAM_WORDS_PER_NODE * node_id);

  memset(node, 0, sizeof(*node));
  node->module = module;
  node->node_id = node_id;
  node->ram_base_words = base_words;

  switch (mod) {
  case CAN_INVERTER:
    node->tx_port = 33U;
    node->tx_pin = 9U;
    node->rx_port = 33U;
    node->rx_pin = 10U;
    node->rxsel = CAN_RXSEL_D;
    break;
  case CAN_GENERAL:
    node->tx_port = 23U;
    node->tx_pin = 2U;
    node->rx_port = 23U;
    node->rx_pin = 3U;
    node->rxsel = CAN_RXSEL_C;
    break;
  case CAN_DV:
    node->tx_port = 0U;
    node->tx_pin = 2U;
    node->rx_port = 0U;
    node->rx_pin = 3U;
    node->rxsel = CAN_RXSEL_A;
    break;
  case CAN_DEBUG:
    node->tx_port = 20U;
    node->tx_pin = 8U;
    node->rx_port = 20U;
    node->rx_pin = 7U;
    node->rxsel = CAN_RXSEL_B;
    break;
  default:
    return -1;
  }

  can_config_pins(node);
  can_config_module(node, baud_rate);

  atomic_store(&node->init_done, 1);
  atomic_store(&node->taken, 0);
  return 0;
}

struct CanNode *hardware_init_can_get_ref_node(const enum CAN_MODULES mod) {
  CanNodeDriver *node = &g_nodes[mod];
  if (mod < __NUM_OF_CAN_MODULES__ && atomic_load(&node->init_done) &&
      !atomic_load(&node->taken)) {
    atomic_store(&node->taken, 1);
    return (struct CanNode *)node;
  }
  return NULL;
}

void hardware_init_can_destroy_ref_node(struct CanNode **restrict self) {
  atomic_store(&((CanNodeDriver *)(*self))->taken, 0);
  *self = NULL;
}

int8_t hardware_read_can(struct CanNode *const restrict self,
                         CanMessage *const restrict mex) {
  CanNodeDriver *node = (CanNodeDriver *)self;
  Ifx_CAN *can = node->module;
  Ifx_CAN_N *n = &can->N[node->node_id];
  uint32_t rxf0_words = node->ram_base_words + CAN_RX_FIFO0_OFF_WORDS;
  uint32_t rx_words_per_elem = 4U;

  if (!atomic_load(&node->init_done)) {
    return -1;
  }

  if (n->RX.F0S.B.F0FL == 0U) {
    return 0;
  }

  {
    uint32_t idx = n->RX.F0S.B.F0GI;
    volatile Ifx_CAN_RXMSG *rx =
        (volatile Ifx_CAN_RXMSG *)&can
            ->RAM[rxf0_words + (idx * rx_words_per_elem)];
    mex->id = rx->R0.B.ID;
    mex->message_size = (uint8_t)rx->R1.B.DLC;
    mex->words[0] =
        ((uint32_t)rx->DB[0].B.DB) | ((uint32_t)rx->DB[1].B.DB << 8) |
        ((uint32_t)rx->DB[2].B.DB << 16) | ((uint32_t)rx->DB[3].B.DB << 24);
    mex->words[1] =
        ((uint32_t)rx->DB[4].B.DB) | ((uint32_t)rx->DB[5].B.DB << 8) |
        ((uint32_t)rx->DB[6].B.DB << 16) | ((uint32_t)rx->DB[7].B.DB << 24);
    n->RX.F0A.B.F0AI = (uint8_t)idx;
  }

  return 1;
}

int8_t hardware_write_can(struct CanNode *const restrict self,
                          const CanMessage *restrict const mex) {
  CanNodeDriver *node = (CanNodeDriver *)self;
  Ifx_CAN *can = node->module;
  Ifx_CAN_N *n = &can->N[node->node_id];
  uint32_t tx_words = node->ram_base_words + CAN_TX_BUF_OFF_WORDS;

  if (!atomic_load(&node->init_done)) {
    return -1;
  }

  if (n->TX.FQS.B.TFQF != 0U) {
    return -1;
  }

  {
    volatile Ifx_CAN_TXMSG *tx = (volatile Ifx_CAN_TXMSG *)&can->RAM[tx_words];
    tx->T0.B.ID = mex->id & 0x1FFFFFFFU;
    tx->T0.B.XTD = 0U;
    tx->T0.B.RTR = 0U;
    tx->T0.B.ESI = 0U;
    tx->T1.B.DLC = mex->message_size & 0xFU;
    tx->T1.B.BRS = 0U;
    tx->T1.B.FDF = 0U;
    tx->T1.B.EFC = 0U;
    tx->T1.B.MM = 0U;

    tx->DB[0].B.DB = (uint8_t)(mex->full_word & 0xFFU);
    tx->DB[1].B.DB = (uint8_t)((mex->full_word >> 8) & 0xFFU);
    tx->DB[2].B.DB = (uint8_t)((mex->full_word >> 16) & 0xFFU);
    tx->DB[3].B.DB = (uint8_t)((mex->full_word >> 24) & 0xFFU);
    tx->DB[4].B.DB = (uint8_t)((mex->full_word >> 32) & 0xFFU);
    tx->DB[5].B.DB = (uint8_t)((mex->full_word >> 40) & 0xFFU);
    tx->DB[6].B.DB = (uint8_t)((mex->full_word >> 48) & 0xFFU);
    tx->DB[7].B.DB = (uint8_t)((mex->full_word >> 56) & 0xFFU);
  }

  n->TX.BAR.U = 1U;
  return 0;
}

struct CanMailbox *hardware_get_mailbox(struct CanNode *const restrict self,
                                        const enum MAILBOX_TYPE type,
                                        uint16_t filter_id,
                                        const uint16_t filter_mask,
                                        const uint16_t mex_size) {
  CanNodeDriver *node = (CanNodeDriver *)self;
  uint16_t mask = (uint16_t)(filter_mask & 0x7FFU);

  if (node == NULL || !atomic_load(&node->init_done)) {
    return NULL;
  }

  for (uint32_t i = 0; i < CAN_MAX_MAILBOXES; i++) {
    if (!atomic_load(&g_mailboxes[i].in_use)) {
      atomic_store(&g_mailboxes[i].in_use, 1);
      g_mailboxes[i].node = node;
      g_mailboxes[i].type = type;
      g_mailboxes[i].filter_id = (uint16_t)(filter_id & 0x7FFU);
      g_mailboxes[i].filter_mask = mask;
      g_mailboxes[i].mex_size = (uint8_t)((mex_size == 0U || mex_size > 8U)
                                              ? 8U
                                              : mex_size);
      return (struct CanMailbox *)&g_mailboxes[i];
    }
  }

  return NULL;
}

int8_t hardware_mailbox_read(struct CanMailbox *const restrict self,
                             CanMessage *const restrict o_mex) {
  CanMailbox *mb = (CanMailbox *)self;
  int8_t res;
  uint16_t mask;

  if (mb == NULL || mb->node == NULL || mb->type == SEND_MAILBOX) {
    return -1;
  }

  res = hardware_read_can((struct CanNode *)mb->node, o_mex);
  if (res <= 0) {
    return res;
  }

  if (o_mex->message_size > mb->mex_size) {
    return -1;
  }

  if (mb->type == RECV_MAILBOX) {
    mask = mb->filter_mask;
    if ((o_mex->id & mask) != (mb->filter_id & mask)) {
      return 0;
    }
  }

  return 1;
}

int8_t hardware_mailbox_send(struct CanMailbox *const restrict self,
                             const uint64_t data) {
  CanMailbox *mb = (CanMailbox *)self;
  CanMessage tx = {0};

  if (mb == NULL || mb->node == NULL || mb->type != SEND_MAILBOX) {
    return -1;
  }

  tx.id = mb->filter_id & 0x7FFU;
  tx.message_size = mb->mex_size;
  tx.full_word = data;

  return hardware_write_can((struct CanNode *)mb->node, &tx);
}

void hardware_free_mailbox_can(struct CanMailbox **restrict self) {
  if (*self == NULL) {
    return;
  }

  atomic_store(&((CanMailbox *)(*self))->in_use, 0);
  *self = NULL;
}
