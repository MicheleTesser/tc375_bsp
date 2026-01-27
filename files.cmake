set( bsp_sources
    machine/wdtcon.c
    bsp/board/bsp_board.c
    bsp/board/board_litekit_TC375_V2/board.c
    bsp/board/board_litekit_TC375_V2/board.c
    bsp/board/board_litekit_TC375_V2/board.c
    bsp/isr/bsp_isr.c
    bsp/isr/bsp_isr_vector_table.c
    bsp/isr/bsp_traps_tc3.c
    bsp/uc/bsp_uc.c
    bsp/uc/uc_tc37/uc_tc37.c
)

set( crt0_sources_tc375
    crt0/uc_tc37_bmhd.c
    crt0/tc3x_crt0.S
)

set( gen_sources
    src/shared_main.c
    driver/adc.c
    driver/can.c
    driver/ethernet.c
    driver/flash_memory.c
    driver/lwip/port/src/Ifx_Lwip_stub.c
    driver/serial.c
    driver/timer.c
    driver/gpio.c
)

set( lwip_sources
    driver/lwip/src/core/altcp.c
    driver/lwip/src/core/altcp_alloc.c
    driver/lwip/src/core/altcp_tcp.c
    driver/lwip/src/core/def.c
    driver/lwip/src/core/dns.c
    driver/lwip/src/core/inet_chksum.c
    driver/lwip/src/core/init.c
    driver/lwip/src/core/ip.c
    driver/lwip/src/core/mem.c
    driver/lwip/src/core/memp.c
    driver/lwip/src/core/netif.c
    driver/lwip/src/core/pbuf.c
    driver/lwip/src/core/raw.c
    driver/lwip/src/core/stats.c
    driver/lwip/src/core/sys.c
    driver/lwip/src/core/tcp.c
    driver/lwip/src/core/tcp_in.c
    driver/lwip/src/core/tcp_out.c
    driver/lwip/src/core/timeouts.c
    driver/lwip/src/core/udp.c
    driver/lwip/src/core/ipv4/autoip.c
    driver/lwip/src/core/ipv4/dhcp.c
    driver/lwip/src/core/ipv4/etharp.c
    driver/lwip/src/core/ipv4/icmp.c
    driver/lwip/src/core/ipv4/igmp.c
    driver/lwip/src/core/ipv4/ip4.c
    driver/lwip/src/core/ipv4/ip4_addr.c
    driver/lwip/src/core/ipv4/ip4_frag.c
    driver/lwip/src/netif/ethernet.c
)

set( freertos_sources
    FreeRTOS-Kernel/list.c
    FreeRTOS-Kernel/queue.c
    FreeRTOS-Kernel/tasks.c
    FreeRTOS-Kernel/portable/MemMang/heap_4.c
    FreeRTOS-Kernel/portable/ThirdParty/Partner-Supported-Ports/Tasking/AURIX_TC3xx/port.c
)
