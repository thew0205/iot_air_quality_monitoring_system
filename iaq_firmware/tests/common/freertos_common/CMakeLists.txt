cmake_minimum_required(VERSION 3.13)

set(TEST_NAME freertos_common_test)
add_library(${TEST_NAME} INTERFACE
    freertos_common_test.cpp
)
target_include_directories(${TEST_NAME} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(${TEST_NAME} INTERFACE PICO_CXX_DISABLE_ALLOCATION_OVERRIDES)

# pull in common dependencies
target_link_libraries(${TEST_NAME} INTERFACE
    CppUTest
    CppUTestExt
    wifi
    freertos_kernel
)


