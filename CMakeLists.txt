cmake_minimum_required(VERSION 3.22.1)

set( PROG_NAME "tc375_bsp_example" )

project(${PROG_NAME} LANGUAGES C ASM)

set(BUILD_CONFIG "LITEKIT_TC375_V2" CACHE STRING "Board type")
set_property(CACHE BUILD_CONFIG PROPERTY STRINGS LITEKIT_TC375_V2)

include( files.cmake )

IF (${BUILD_CONFIG} STREQUAL "LITEKIT_TC375_V2")
    set(crt0_sources ${crt0_sources_tc375} )
ENDIF()

add_executable(${PROG_NAME} ${bsp_sources} ${crt0_sources} ${gen_sources} ${freertos_sources} )

target_include_directories(${PROG_NAME} PRIVATE
    "./bsp"
    "./"
    "./FreeRTOS-Kernel/include"
    "./FreeRTOS-Kernel/portable/ThirdParty/Partner-Supported-Ports/Tasking/AURIX_TC3xx"
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
