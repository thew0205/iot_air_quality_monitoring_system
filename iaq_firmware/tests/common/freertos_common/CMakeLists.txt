cmake_minimum_required(VERSION 3.13)

set(TEST_NAME freertos_common_test)
add_library(${TEST_NAME}
    freertos_common_test.cpp
    freertos_port.c
)
target_include_directories(${TEST_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(${TEST_NAME} PUBLIC PICO_CXX_DISABLE_ALLOCATION_OVERRIDES)

# pull in common dependencies
target_link_libraries(${TEST_NAME} PUBLIC
    CppUTest
    CppUTestExt
    wifi
    FreeRTOS-Kernel-Heap4 # FreeRTOS kernel and dynamic heap
    FreeRTOS-Kernel
)


