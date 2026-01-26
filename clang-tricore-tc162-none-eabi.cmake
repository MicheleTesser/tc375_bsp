set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR tricore)

# Compilation check
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)


# Binutil list
set(CMAKE_C_COMPILER    "$ENV{TRICORE_LLVM}/bin/clang")
set(CMAKE_ASM_COMPILER  "$ENV{TRICORE_LLVM}/bin/clang")
set(CMAKE_CXX_COMPILER  "$ENV{TRICORE_LLVM}/bin/clang")
set(CMAKE_OBJCOPY       "$ENV{TRICORE_LLVM}/bin/llvm-objcopy")
set(CMAKE_SIZE          "$ENV{TRICORE_LLVM}/bin/llvm-size")
set(CMAKE_LINKER        "$ENV{TRICORE_LLVM}/bin/clang")
set(CMAKE_AR            "$ENV{TRICORE_LLVM}/bin/llvm-ar")

set(CMAKE_EXECUTABLE_SUFFIX_ASM ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C   ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_CXX ".elf")

# Default option settings
set(CMAKE_C_FLAGS "-march=tc162")
set(CMAKE_ASM_FLAGS "-march=tc162")
set(CMAKE_CXX_FLAGS "-march=tc162 -fno-exceptions")
set(CMAKE_EXE_LINKER_FLAGS "-march=tc162 -Wl,--cref -Wl,-n" )


set(CMAKE_C_FLAGS_DEBUG "-fomit-frame-pointer -g -O0 " )
set(CMAKE_ASM_FLAGS_DEBUG " ")
set(CMAKE_CXX_FLAGS_DEBUG "-fomit-frame-pointer -g -O0")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG " ")

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

