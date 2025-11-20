# Generated Cmake Pico project file
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(iaq_firmware_src C CXX ASM)


if(NOT DEFINED IOT_FIRMWARE_ROOT_DIR OR IOT_FIRMWARE_ROOT_DIR STREQUAL "" OR NOT EXISTS "${IOT_FIRMWARE_ROOT_DIR}/src")
    message(FATAL_ERROR "IOT_FIRMWARE_ROOT_DIR must be defined and must point to the root of the project where src folder and the include folders are located")
endif()


add_subdirectory(${IOT_FIRMWARE_ROOT_DIR}/src/sensors)
add_subdirectory(${IOT_FIRMWARE_ROOT_DIR}/src/fat_sd_card)

