# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)

set(TESTS OFF CACHE BOOL "Disable tests" FORCE)

project(blink_app C CXX ASM)



# Add executable. Default name is the project name, version 0.1

set(FREERTOS_KERNEL_PATH  "${IOT_FIRMWARE_ROOT_DIR}/third_party/FreeRTOS-Kernel")

set(FREERTOS_CONFIG_FILE_DIRECTORY "${IOT_FIRMWARE_ROOT_DIR}/tests/common" CACHE STRING "Local Config")

add_subdirectory(${FREERTOS_KERNEL_PATH} ${CMAKE_CURRENT_BINARY_DIR}/third_party/FreeRTOS-Kernel)

add_subdirectory(${IOT_FIRMWARE_ROOT_DIR}/src ${CMAKE_CURRENT_BINARY_DIR}/src)

add_subdirectory(${IOT_FIRMWARE_ROOT_DIR}/tests/third_party/cpputest ${CMAKE_CURRENT_BINARY_DIR}/third_party/cpputest)

add_subdirectory(${IOT_FIRMWARE_ROOT_DIR}/tests/common ${CMAKE_CURRENT_BINARY_DIR}/tests/common)


add_subdirectory(freertos_host)