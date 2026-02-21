cmake_minimum_required(VERSION 3.13)


set(TEST_NAME wolfssl_arduino_integration_target_test)

set(WIFI_SSID "iaq_router_airtel")
set(WIFI_PASSWORD "1234567890")


# Add executable. Default name is the project name, version 0.1

add_executable(${TEST_NAME}
        wolfssl_arduino.c
        # freertos_port.c
)
target_compile_definitions(${TEST_NAME} PRIVATE
        WIFI_SSID=\"${WIFI_SSID}\"
        WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
)
target_include_directories(${TEST_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
target_link_libraries(${TEST_NAME}
        cmsis_core
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
        # iaq_utils
        pico_cyw43_arch_lwip_sys_freertos
        pico_stdlib
        wolfssl
)

target_compile_definitions(pico_cyw43_arch_lwip_sys_freertos INTERFACE
        LWIP_TIMEVAL_PRIVATE=0
)

pico_enable_stdio_usb(${TEST_NAME} 1)
pico_enable_stdio_uart(${TEST_NAME} 1)

# target_compile_definitions(${TEST_NAME} PRIVATE
#         PICO_DEFAULT_UART=0
#         PICO_DEFAULT_UART_TX_PIN=16
#         PICO_DEFAULT_UART_RX_PIN=17
# )
pico_add_extra_outputs(${TEST_NAME})

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--print-memory-usage")
