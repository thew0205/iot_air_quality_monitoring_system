# Generated Cmake Pico project file
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(iaq_firmware C CXX ASM)

set(IOT_FIRMWARE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(src)
