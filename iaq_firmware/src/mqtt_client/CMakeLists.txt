# Generated Cmake Pico project file
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(mqtt_client C CXX ASM)

set(SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/mqtt_function.c)

if(PICO_PLATFORM STREQUAL "rp2040" OR PICO_PLATFORM STREQUAL "rp2350")
    # list(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/platforms/pico/adc_interface.cpp)
elseif(PICO_PLATFORM STREQUAL "host")
    # list(APPEND SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/platforms/host/adc_interface.cpp)
endif()


target_compile_definitions(pico_cyw43_arch_lwip_sys_freertos INTERFACE
        # LWIP_TIMEVAL_PRIVATE=0
)


set(LINK_LIBRARIES
    pico_stdlib
    FreeRTOS-Kernel
    FreeRTOS-Kernel-Heap4
    iaq_utils
    pico_cyw43_arch_lwip_sys_freertos
    pico_lwip_mqtt
    pico_mbedtls
    pico_lwip_mbedtls
)

if(PICO_PLATFORM STREQUAL "rp2040" OR PICO_PLATFORM STREQUAL "rp2350")
    list(APPEND LINK_LIBRARIES cmsis_core)
endif()

add_library(${PROJECT_NAME}
    ${SRC_FILES}
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    ${LINK_LIBRARIES}
)

