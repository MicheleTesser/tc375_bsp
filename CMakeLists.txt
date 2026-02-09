cmake_minimum_required(VERSION 3.20)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR tricore)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

project(litekit_tc375_example C ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(CMAKE_EXE_LINKER_FLAGS MATCHES "--cref")
  string(REPLACE "-Wl,--cref" "" CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
  string(REPLACE "--cref" "" CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
endif()

set(TRICORE_CPU_FLAGS "-mcpu=tc3xx" CACHE STRING "Extra CPU-specific flags (e.g. -mcpu=tc3xx)")
separate_arguments(TRICORE_CPU_FLAGS)

set(LINKER_MEMORY_SCRIPT "${CMAKE_SOURCE_DIR}/ld/tc37xA_memory.ld")
set(LINKER_DEPENDS
  "${CMAKE_SOURCE_DIR}/ld/tc37xA_memory.ld"
  "${CMAKE_SOURCE_DIR}/ld/tc37x_bsp_example_llvm.ld"
)

set(BSP_SOURCES
  "${CMAKE_SOURCE_DIR}/shared_main.c"
  "${CMAKE_SOURCE_DIR}/machine/wdtcon.c"
  "${CMAKE_SOURCE_DIR}/crt0/tc3x_crt0.S"
  "${CMAKE_SOURCE_DIR}/crt0/uc_tc37_bmhd.c"
  "${CMAKE_SOURCE_DIR}/UART/UART_Logging.c"
)

file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/src/cores/**/*.c"
)

file(GLOB_RECURSE ILLD_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/illd_release_tc3x/src/BaseSw/iLLD/TC3xx/Tricore/**/*.c"
  "${CMAKE_SOURCE_DIR}/illd_release_tc3x/src/BaseSw/Infra/Platform/Tricore/Compilers/*.c"
  "${CMAKE_SOURCE_DIR}/illd_release_tc3x/src/BaseSw/Infra/Ssw/TC3xx/Tricore/*.c"
  "${CMAKE_SOURCE_DIR}/Ethernet/lwip/port/src/*.c"
  "${CMAKE_SOURCE_DIR}/Ethernet/Phy_Dp83825i/*.c"
)
set(LWIP_SOURCES "")
file(GLOB LWIP_CORE_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/Ethernet/lwip/src/core/*.c"
)
file(GLOB LWIP_CORE_IPV4_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/Ethernet/lwip/src/core/ipv4/*.c"
)
file(GLOB LWIP_NETIF_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/Ethernet/lwip/src/netif/ethernet.c"
)
list(APPEND LWIP_SOURCES
  ${LWIP_CORE_SOURCES}
  ${LWIP_CORE_IPV4_SOURCES}
  ${LWIP_NETIF_SOURCES}
)
file(GLOB DRIVER_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/driver_imp/*.c"
)

set(BOARD_DBC_SOURCES
  "${CMAKE_SOURCE_DIR}/lib/board_dbc/dbc/out_lib/can1/can1.c"
  "${CMAKE_SOURCE_DIR}/lib/board_dbc/dbc/out_lib/can2/can2.c"
  "${CMAKE_SOURCE_DIR}/lib/board_dbc/dbc/out_lib/can3/can3.c"
)

set(DPS_SOURCES
  "${CMAKE_SOURCE_DIR}/lib/DPS/src/common/dps_messages.c"
  "${CMAKE_SOURCE_DIR}/lib/DPS/src/master/dps_master.c"
  "${CMAKE_SOURCE_DIR}/lib/DPS/src/slave/dps_slave.c"
  "${CMAKE_SOURCE_DIR}/lib/DPS/lib/c_vector/c_vector.c"
)

set(TV_SOURCES
  "${CMAKE_SOURCE_DIR}/lib/tv_2025/ControlSystems.c"
  "${CMAKE_SOURCE_DIR}/lib/tv_2025/ControlSystems_data.c"
  "${CMAKE_SOURCE_DIR}/lib/tv_2025/rtGetInf.c"
  "${CMAKE_SOURCE_DIR}/lib/tv_2025/rtGetNaN.c"
  "${CMAKE_SOURCE_DIR}/lib/tv_2025/rt_nonfinite.c"
)

file(GLOB_RECURSE ILLD_INCLUDE_CANDIDATES CONFIGURE_DEPENDS
  LIST_DIRECTORIES true
  "${CMAKE_SOURCE_DIR}/illd_release_tc3x/src/BaseSw/iLLD/TC3xx/Tricore/*"
)
set(ILLD_INCLUDE_DIRS "")
foreach(path IN LISTS ILLD_INCLUDE_CANDIDATES)
  if(IS_DIRECTORY "${path}")
    list(APPEND ILLD_INCLUDE_DIRS "${path}")
  endif()
endforeach()
list(REMOVE_DUPLICATES ILLD_INCLUDE_DIRS)

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE
  ${BSP_SOURCES}
  ${APP_SOURCES}
  ${ILLD_SOURCES}
  ${LWIP_SOURCES}
  ${DRIVER_SOURCES}
  ${BOARD_DBC_SOURCES}
  ${DPS_SOURCES}
  ${TV_SOURCES}
)

target_include_directories(${PROJECT_NAME} PRIVATE
  "${CMAKE_SOURCE_DIR}"
  "${CMAKE_SOURCE_DIR}/Configurations"
  "${CMAKE_SOURCE_DIR}/src"
  "${CMAKE_SOURCE_DIR}/machine"
  "${CMAKE_SOURCE_DIR}/components"
  "${CMAKE_SOURCE_DIR}/UART"
  "${CMAKE_SOURCE_DIR}/lib"
  "${CMAKE_SOURCE_DIR}/lib/board_dbc/dbc/out_lib/can1"
  "${CMAKE_SOURCE_DIR}/lib/board_dbc/dbc/out_lib/can2"
  "${CMAKE_SOURCE_DIR}/lib/board_dbc/dbc/out_lib/can3"
  "${CMAKE_SOURCE_DIR}/lib/DPS"
  "${CMAKE_SOURCE_DIR}/lib/DPS/src/common"
  "${CMAKE_SOURCE_DIR}/lib/DPS/src/master"
  "${CMAKE_SOURCE_DIR}/lib/DPS/src/slave"
  "${CMAKE_SOURCE_DIR}/lib/DPS/lib/c_vector"
  "${CMAKE_SOURCE_DIR}/lib/tv_2025"
  "${CMAKE_SOURCE_DIR}/Ethernet/lwip/port/include"
  "${CMAKE_SOURCE_DIR}/Ethernet/lwip/src/include"
  "${CMAKE_SOURCE_DIR}/Ethernet/lwip/src/include/lwip"
  "${CMAKE_SOURCE_DIR}/Ethernet/Phy_Dp83825i/"
  "${CMAKE_SOURCE_DIR}/illd_release_tc3x/src/BaseSw/iLLD/TC3xx/Tricore"
  "${CMAKE_SOURCE_DIR}/illd_release_tc3x/src/BaseSw/iLLD/TC3xx/Tricore/_impl"
  "${CMAKE_SOURCE_DIR}/illd_release_tc3x/src/BaseSw/Service/CpuGeneric"
  "${CMAKE_SOURCE_DIR}/illd_release_tc3x/src/BaseSw/Infra/Platform"
  "${CMAKE_SOURCE_DIR}/illd_release_tc3x/src/BaseSw/Infra/Ssw/TC3xx/Tricore"
  "${CMAKE_SOURCE_DIR}/illd_release_tc3x/src/BaseSw/Infra/Sfr/TC37x"
  "${CMAKE_SOURCE_DIR}/illd_release_tc3x/src/BaseSw/iLLD/TC3xx/Tricore/_PinMap"
  "${CMAKE_SOURCE_DIR}/illd_release_tc3x/src/BaseSw/iLLD/TC3xx/Tricore/_PinMap/TC37x"
  "${CMAKE_SOURCE_DIR}/illd_release_tc3x/src/BaseSw/iLLD/TC3xx/Tricore/Scu/Std"
  "${CMAKE_SOURCE_DIR}/illd_release_tc3x/src/BaseSw/Service/CpuGeneric/SysSe/Bsp"
  "${CMAKE_SOURCE_DIR}/illd_release_tc3x/src/BaseSw/iLLD/TC3xx/Tricore/Evadc/Adc"
  "${CMAKE_SOURCE_DIR}/illd_release_tc3x/src/BaseSw/Service/CpuGeneric/SysSe/Comm/"
  ${ILLD_INCLUDE_DIRS}
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
  __TC37XX__
  DEVICE_TC37X
  IFX_PIN_PACKAGE_LQFP176
  LITEKIT_TC375_V2
  IFX_USE_COMPILER_CONSTRUCTED_INT
)

target_compile_options(${PROJECT_NAME} PRIVATE
  $<$<COMPILE_LANGUAGE:C>:-g -O2 -fno-common -ffunction-sections -fdata-sections -Wall ${TRICORE_CPU_FLAGS}>
  $<$<AND:$<COMPILE_LANGUAGE:C>,$<C_COMPILER_ID:GNU>>:-fstrict-volatile-bitfields>
  $<$<COMPILE_LANGUAGE:ASM>:-g ${TRICORE_CPU_FLAGS}>
  $<$<AND:$<COMPILE_LANGUAGE:C>,$<C_COMPILER_ID:Clang>>:-Qunused-arguments>
)

target_link_options(${PROJECT_NAME} PRIVATE
  -Wl,-T,${LINKER_MEMORY_SCRIPT}
  -Wl,-L,${CMAKE_SOURCE_DIR}/ld
  -Wl,--gc-sections
  -Wl,-n
  ${TRICORE_CPU_FLAGS}
)

set_property(TARGET ${PROJECT_NAME} PROPERTY LINK_DEPENDS "${LINKER_DEPENDS}")
