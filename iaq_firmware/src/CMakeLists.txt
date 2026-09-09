# Generated Cmake Pico project file
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(iaq_firmware_src C CXX ASM)


if(NOT DEFINED IOT_FIRMWARE_ROOT_DIR OR IOT_FIRMWARE_ROOT_DIR STREQUAL "" OR NOT EXISTS "${IOT_FIRMWARE_ROOT_DIR}/src")
    message(FATAL_ERROR "IOT_FIRMWARE_ROOT_DIR must be defined and must point to the root of the project where src folder and the include folders are located")
endif()

set(DS3231_LIB_PATH "${IOT_FIRMWARE_ROOT_DIR}/third_party/RP2040DS3231RTC")
include(${IOT_FIRMWARE_ROOT_DIR}/third_party/RP2040DS3231RTC/ds3231_lib_import.cmake)
include(wolfssl_include.cmake)

add_subdirectory(${IOT_FIRMWARE_ROOT_DIR}/third_party/memcpy_smart_ptr ${CMAKE_BINARY_DIR}/memcpy_smart_ptr)

include(${IOT_FIRMWARE_ROOT_DIR}/third_party/coreMQTT/mqttFilePaths.cmake)
add_library(core_mqtt STATIC)
target_sources(core_mqtt PRIVATE ${MQTT_SOURCES}
    ${MQTT_SERIALIZER_SOURCES})
target_include_directories(core_mqtt
    PUBLIC ${IOT_FIRMWARE_ROOT_DIR}/include
    ${MQTT_INCLUDE_PUBLIC_DIRS}
)
target_link_libraries(core_mqtt PRIVATE iaq_utils)

add_subdirectory(${IOT_FIRMWARE_ROOT_DIR}/src/sensors)
add_subdirectory(${IOT_FIRMWARE_ROOT_DIR}/src/fat_sd_card)
add_subdirectory(${IOT_FIRMWARE_ROOT_DIR}/src/iaq_utils)
add_subdirectory(${IOT_FIRMWARE_ROOT_DIR}/src/rtc)
if(PICO_PLATFORM STREQUAL "rp2040" OR PICO_PLATFORM STREQUAL "rp2350")
add_subdirectory(${IOT_FIRMWARE_ROOT_DIR}/src/mqtt_client)
endif()

