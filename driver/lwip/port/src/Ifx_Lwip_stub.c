#include <stddef.h>
#include <string.h>

#include "lwip/err.h"
#include "lwip/init.h"
#include "lwip/ip4_addr.h"
#include "lwip/netif.h"
#include "lwip/pbuf.h"
#include "lwip/timeouts.h"
#include "netif/etharp.h"
#include "netif/ethernet.h"

volatile uint32_t g_TickCount_1ms = 0U;

static struct netif g_lwip_netif;

static err_t ifx_lwip_linkoutput(struct netif *netif, struct pbuf *p)
{
    (void)netif;
    if (p != NULL)
    {
        pbuf_free(p);
    }
    return ERR_OK;
}

static err_t ifx_lwip_netif_init(struct netif *netif)
{
    if (netif == NULL)
    {
        return ERR_VAL;
    }

    netif->name[0] = 'e';
    netif->name[1] = 'n';
    netif->output = etharp_output;
    netif->linkoutput = ifx_lwip_linkoutput;
    netif->mtu = 1500U;
    netif->hwaddr_len = ETH_HWADDR_LEN;
    netif->flags = NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP | NETIF_FLAG_LINK_UP;

    return ERR_OK;
}

void Ifx_Lwip_init(struct eth_addr ethAddr)
{
    ip4_addr_t ipaddr;
    ip4_addr_t netmask;
    ip4_addr_t gw;

    lwip_init();

    ip4_addr_set_zero(&ipaddr);
    ip4_addr_set_zero(&netmask);
    ip4_addr_set_zero(&gw);

    (void)netif_add(&g_lwip_netif, &ipaddr, &netmask, &gw, NULL, ifx_lwip_netif_init, ethernet_input);
    memcpy(g_lwip_netif.hwaddr, ethAddr.addr, ETH_HWADDR_LEN);
    netif_set_up(&g_lwip_netif);
    netif_set_link_up(&g_lwip_netif);
}

void Ifx_Lwip_onTimerTick(void)
{
}

void Ifx_Lwip_pollTimerFlags(void)
{
    sys_check_timeouts();
}

void Ifx_Lwip_pollReceiveFlags(void)
{
}

struct netif *Ifx_Lwip_getNetIf(void)
{
    return &g_lwip_netif;
}

u32_t sys_now(void)
{
    return (u32_t)g_TickCount_1ms;
}

u32_t sys_jiffies(void)
{
    return (u32_t)g_TickCount_1ms;
}

s8_t Ifx_Lwip_printf(const char *format, ...)
{
    (void)format;
    return 0;
}
