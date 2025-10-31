# Generated Cmake Pico project file
cmake_minimum_required(VERSION 3.13)

set(IOT_FIRMWARE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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
set(picotoolVersion 2.2.0-a4)
set(picoVscode ${USERHOME}/.pico-sdk/cmake/pico-vscode.cmake)
if (EXISTS ${picoVscode})
    include(${picoVscode})
endif()
# ====================================================================================
set(PICO_BOARD pico CACHE STRING "Board type")
set(PICO_PLATFORM rp2350)
# set(PICO_PLATFORM host CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

set(TESTS OFF CACHE BOOL "Disable tests" FORCE)

project(iaq_firmware_app C CXX ASM)


# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

set(IOT_FIRMWARE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(src)

add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE 
    pico_stdlib
    sensors
    wifi
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${IOT_FIRMWARE_ROOT_DIR}/include
)

#output options
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 1)

#ouputs .bin, .elf, .uf2 files
pico_add_extra_outputs(${PROJECT_NAME})