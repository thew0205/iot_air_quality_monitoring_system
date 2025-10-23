# Generated Cmake Pico project file
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(sensors C CXX ASM)

add_library(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/sensors.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

target_link_libraries(sensors
    pico_stdlib
    hardware_uart
    hardware_gpio
    hardware_adc
)