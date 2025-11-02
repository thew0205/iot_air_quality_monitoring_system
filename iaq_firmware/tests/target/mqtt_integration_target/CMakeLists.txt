cmake_minimum_required(VERSION 3.13)


set(TEST_NAME mqtt_integration_target_test)


set(MQTT_SERVER "aj8uipcillvb-ats.iot.eu-west-2.amazonaws.com") # Change this to the host name of your MQTT server

set(WIFI_SSID "iaq_wifi")
set(WIFI_PASSWORD "1234567890")

# Add executable. Default name is the project name, version 0.1

add_executable(${TEST_NAME}
        mqtt_client.c
        freertos_port.c
)
target_compile_definitions(${TEST_NAME} PRIVATE
        WIFI_SSID=\"${WIFI_SSID}\"
        WIFI_PASSWORD=\"${WIFI_PASSWORD}\"
        MQTT_SERVER=\"${MQTT_SERVER}\"
)
target_include_directories(${TEST_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
)
target_link_libraries(${TEST_NAME}
        cmsis_core
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
        pico_cyw43_arch_lwip_sys_freertos
        pico_stdlib
        pico_lwip_mqtt
        pico_mbedtls
        pico_lwip_mbedtls
        hardware_adc
)

target_compile_definitions(pico_cyw43_arch_lwip_sys_freertos INTERFACE
        LWIP_TIMEVAL_PRIVATE=0
)
target_compile_definitions(${TEST_NAME} PRIVATE
    MQTT_CERT_INC=\"${MQTT_CERT_INC}\" # contains the tls certificates for MQTT_SERVER needed by the client
    ALTCP_MBEDTLS_AUTHMODE=MBEDTLS_SSL_VERIFY_REQUIRED
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
