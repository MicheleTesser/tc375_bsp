#include <stdint.h>
#include <string.h>

#include "ethernet.h"
#include "serial.h"

#undef DEBUG

#include "bsp.h"
#include "lwip/init.h"
#include "lwip/ip.h"
#include "lwip/ip_addr.h"
#include "lwip/netif.h"
#include "lwip/prot/ethernet.h"
#include "lwip/timeouts.h"
#include "lwip/udp.h"

#ifndef ISR_PRIORITY_OS_TICK
#define ISR_PRIORITY_OS_TICK 1U
#endif

#ifndef CPU_WHICH_SERVICE_ETHERNET
#define CPU_WHICH_SERVICE_ETHERNET 2U
#endif

#define ETH_STM_TICKS_PER_MS 100000U

extern void Ifx_Lwip_init(struct eth_addr ethAddr);
extern void Ifx_Lwip_onTimerTick(void);
extern void Ifx_Lwip_pollTimerFlags(void);
extern void Ifx_Lwip_pollReceiveFlags(void);
extern struct netif *Ifx_Lwip_getNetIf(void);
extern volatile uint32_t g_TickCount_1ms;

static void updateLwIPStackISR(void);

struct EthernetNodeIpv4_t {
  const char *addr;
  uint32_t port;
  uint8_t padding[5];
};

union EthernetNodeIpv4_h_t_conv {
  EthernetNodeIpv4_h *hidden;
  struct EthernetNodeIpv4_t *clear;
};

union EthernetNodeIpv4_h_t_conv_const {
  const EthernetNodeIpv4_h *hidden;
  const struct EthernetNodeIpv4_t *clear;
};

char __assert_size_ethernet
    [sizeof(EthernetNodeIpv4_h) == sizeof(struct EthernetNodeIpv4_t) ? 1 : -1];
char __assert_align_ethernet[_Alignof(EthernetNodeIpv4_h) ==
                                     _Alignof(struct EthernetNodeIpv4_t)
                                 ? 1
                                 : -1];

static uint8_t INIT_DONE = 0;
static struct udp_pcb *G_PCB; /* Pointer to the TCP protocol control block    */

int8_t hardware_ethernet_udp_init(EthernetNodeIpv4_h *const restrict self
                                  __attribute__((__unused__)),
                                  const IpAddrIpV4Port *const addr
                                  __attribute__((__unused__))) {
  union EthernetNodeIpv4_h_t_conv conv = {self};
  struct EthernetNodeIpv4_t *p_self = conv.clear;
  ip_addr_t remote_ip;
  if (!INIT_DONE) {
#ifndef BSP_ISR_SUPPORT_DISABLED
    bsp_isr_RegisterHandler(CPU_WHICH_SERVICE_ETHERNET, updateLwIPStackISR,
                            ISR_PRIORITY_OS_TICK);
#endif

    bsp_uc_intc_stm_SetSRC(0U, CPU_WHICH_SERVICE_ETHERNET,
                           ISR_PRIORITY_OS_TICK);
    bsp_uc_stm_EnableChannelIsr(0U);
    bsp_uc_stm_ReloadChannel(
        0U, ETH_STM_TICKS_PER_MS *
                10U); /* First interrupt shall occur after 10 ms      */

    /* Define a MAC Address */
    struct eth_addr ethAddr;
    ethAddr.addr[0] = 0xDE;
    ethAddr.addr[1] = 0xAD;
    ethAddr.addr[2] = 0xBE;
    ethAddr.addr[3] = 0xEF;
    ethAddr.addr[4] = 0xFE;
    ethAddr.addr[5] = 0xED;

    Ifx_Lwip_init(ethAddr); /* Initialize LwIP with the MAC address */

    INIT_DONE = 1;
  }

  G_PCB = udp_new();
  if (!G_PCB) {
    serial_write_str("error init udb");
    return -1;
  }
  udp_bind_netif(G_PCB, Ifx_Lwip_getNetIf());

  ipaddr_aton(addr->addr, &remote_ip);
  p_self->addr = addr->addr;
  p_self->port = addr->port;

  return 0;
}

void hardware_ethernet_update(void) {
  Ifx_Lwip_pollTimerFlags(); /* Poll LwIP timers and trigger protocols execution
                                if required */
  Ifx_Lwip_pollReceiveFlags(); /* Receive data package through ETH */
}

int8_t hardware_ethernet_udp_send(const EthernetNodeIpv4_h *const restrict self
                                  __attribute__((__unused__)),
                                  const UdpIpv4Mex *const restrict data
                                  __attribute__((__unused__))) {
  union EthernetNodeIpv4_h_t_conv_const conv = {self};
  const struct EthernetNodeIpv4_t *p_self = conv.clear;
  ip_addr_t dest_ip = {0};

  if (!INIT_DONE) {
    return -1;
  }

  struct pbuf packet;
  memset(&packet, 0, sizeof(packet));
  packet.len = data->data_length;
  packet.tot_len = data->data_length;
  packet.payload = (void *)data->raw_data;

  ipaddr_aton(p_self->addr, &dest_ip);
  udp_sendto(G_PCB, &packet, &dest_ip, p_self->port);

  return 0;
}

void hardware_ethernet_udp_free(EthernetNodeIpv4_h *self
                                __attribute__((__unused__))) {
  if (G_PCB != NULL) {
    udp_remove(G_PCB);
    G_PCB = NULL;
  }
  return;
}

/* ISR to update LwIP stack */
void updateLwIPStackISR(void) {
  /* Configure STM to generate an interrupt in 1 ms */
  bsp_uc_stm_ClearChannelIsrFlag(0U);
  bsp_uc_stm_ReloadChannel(0U, ETH_STM_TICKS_PER_MS);

  g_TickCount_1ms++; /* Increase LwIP system time */

  Ifx_Lwip_onTimerTick(); /* Every 1 ms LwIP timers are increased for all the
                             enabled * protocols (ARP, TCP, DHCP, LINK) */
}
