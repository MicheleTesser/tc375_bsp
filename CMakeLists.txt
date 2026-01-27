cmake_minimum_required(VERSION 3.22.1)

set( PROG_NAME "tc375_bsp_example" )

project(${PROG_NAME} LANGUAGES C ASM)

set(BUILD_CONFIG "LITEKIT_TC375_V2" CACHE STRING "Board type")
set_property(CACHE BUILD_CONFIG PROPERTY STRINGS LITEKIT_TC375_V2)

include( files.cmake )

IF (${BUILD_CONFIG} STREQUAL "LITEKIT_TC375_V2")
    set(crt0_sources ${crt0_sources_tc375} )
ENDIF()

set_source_files_properties(${freertos_sources} PROPERTIES
    COMPILE_OPTIONS "-Wno-unknown-attributes"
)

set_source_files_properties(${bsp_sources} PROPERTIES
    COMPILE_OPTIONS "-Wno-extra-semi"
)

add_executable(${PROG_NAME} ${bsp_sources} ${crt0_sources} ${gen_sources} ${freertos_sources} ${lwip_sources} )

target_include_directories(${PROG_NAME} PRIVATE
    "./bsp"
    "./"
    "./bsp/uc/uc_tc37/TC37xPD_RegCHeaders/_Reg"
    "./driver/Configurations"
    "./driver/lwip/src/include"
    "./driver/lwip/port/include"
    "./FreeRTOS-Kernel/include"
    "./FreeRTOS-Kernel/portable/ThirdParty/Partner-Supported-Ports/Tasking/AURIX_TC3xx"
)

target_compile_options(${PROG_NAME} PRIVATE
    $<$<C_COMPILER_ID:Clang>:-Wno-unknown-pragmas>
)

target_link_directories(${PROG_NAME} PRIVATE
    "./ld"
)

target_link_libraries(${PROG_NAME} PRIVATE
)

target_link_options(${PROG_NAME} PRIVATE
    -Wl,-Map=${PROG_NAME}.map
    -Wl,--gc-sections
    -Wl,--cref
)

IF (${BUILD_CONFIG} STREQUAL "LITEKIT_TC375_V2")
    target_compile_options( ${PROG_NAME} PRIVATE
        -DLITEKIT_TC375_V2
        -D__TC37XX__
        -mfloat-abi=hh32
        -DTC375A
        -DDEBUG_BUILD
        -Wno-fixed-enum-extension
        -Wno-strict-prototypes
        -pedantic
        -Wall
        -Wuninitialized
        -Wconditional-uninitialized
    )
    target_link_options(${PROG_NAME} PRIVATE
        -mfloat-abi=hh32
        -Wl,-Ttc37xA_memory.ld
    )
ENDIF()
