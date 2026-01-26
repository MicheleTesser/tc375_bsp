
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
)
