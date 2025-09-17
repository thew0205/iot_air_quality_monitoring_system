# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)

# == DO NOT EDIT THE FOLLOWING LINES for the Raspberry Pi Pico VS Code Extension to work ==
if(WIN32)
    set(USERHOME $ENV{USERPROFILE})
else()
    set(USERHOME $ENV{HOME})
endif()
set(sdkVersion 2.2.0)
set(toolchainVersion 14_2_Rel1)
set(picotoolVersion 2.2.0)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if(EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

set(TESTS OFF CACHE BOOL "Disable tests" FORCE)


project(blink_p C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

set(FREERTOS_KERNEL_PATH "${IOT_FIRMWARE_ROOT_DIR}/third_party/FreeRTOS-Kernel")

set(FREERTOS_CONFIG_FILE_DIRECTORY "${IOT_FIRMWARE_ROOT_DIR}/tests/common" CACHE STRING "Local Config")
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

target_link_libraries(FreeRTOS-Kernel INTERFACE pico_stdlib hardware_clocks cmsis_core)


add_subdirectory(${IOT_FIRMWARE_ROOT_DIR}/third_party/FreeRTOS-FAT-CLI-for-RPi-Pico/src/FreeRTOS+FAT+CLI ${CMAKE_BINARY_DIR}/FreeRTOS-FAT-CLI-for-RPi-Pico/src/FreeRTOS+FAT+CLI)

add_subdirectory(${IOT_FIRMWARE_ROOT_DIR}/src ${CMAKE_BINARY_DIR}/src)

add_subdirectory(${IOT_FIRMWARE_ROOT_DIR}/tests/third_party/cpputest ${CMAKE_BINARY_DIR}/third_party/cpputest)

add_subdirectory(${IOT_FIRMWARE_ROOT_DIR}/tests/common ${CMAKE_CURRENT_BINARY_DIR}/tests/common)

add_subdirectory(freertos_target)
add_subdirectory(freertos_fat_target)
