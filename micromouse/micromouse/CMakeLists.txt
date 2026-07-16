cmake_minimum_required(VERSION 3.20)
project(micromouse C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    src/main.c
    src/core/maze.c
    src/core/robot.c
    src/core/sensor.c
    src/core/motor.c
    src/algorithms/flood_fill.c
    src/utils/pid.c
)

# Header directories
set(INCLUDE_DIRS
    src
    src/core
    src/algorithms
    src/utils
)

# Create executable
add_executable(micromouse ${SOURCES})

# Include directories
target_include_directories(micromouse PRIVATE ${INCLUDE_DIRS})

# Compiler flags
target_compile_options(micromouse PRIVATE
    -Wall
    -Wextra
    -O2
    -g
)

# Debug build
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(micromouse PRIVATE DEBUG_ENABLED=1)
    target_compile_options(micromouse PRIVATE -O0 -g3)
endif()

# Release build
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(micromouse PRIVATE -Os)
    target_compile_definitions(micromouse PRIVATE DEBUG_ENABLED=0)
endif()

# Testing
option(TESTING "Enable testing" OFF)
if(TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

# ARM cross-compilation (for actual hardware)
if(CMAKE_CROSSCOMPILING)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-m4 -mthumb")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -specs=nosys.specs")
    
    # Linker script for STM32F405
    # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T ${CMAKE_SOURCE_DIR}/linker/stm32f405.ld")
endif()
