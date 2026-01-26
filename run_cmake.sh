#!/bin/bash

set -e

BUILD_CONFIG=${1:-"LITEKIT_TC375_V2"}
BUILD_TYPE=debug
rm -rf ".build_${BUILD_CONFIG}_${BUILD_TYPE}"
cmake -DCMAKE_TOOLCHAIN_FILE="clang-tricore-tc162-none-eabi.cmake" \
      -DBUILD_CONFIG=${BUILD_CONFIG} \
      -B ".build_${BUILD_CONFIG}_${BUILD_TYPE}" \
      -G Ninja --warn-uninitialized -Wdev -DCMAKE_BUILD_TYPE=${BUILD_TYPE}
ninja -C ".build_${BUILD_CONFIG}_${BUILD_TYPE}"
llvm-objcopy -O ihex ".build_${BUILD_CONFIG}_${BUILD_TYPE}/tc375_bsp_example.elf" ".build_${BUILD_CONFIG}_${BUILD_TYPE}/tc375_bsp_example.hex"
