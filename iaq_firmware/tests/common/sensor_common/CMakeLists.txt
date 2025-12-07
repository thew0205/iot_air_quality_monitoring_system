cmake_minimum_required(VERSION 3.13)

set(TEST_NAME sensor_common_test)

add_executable(${TEST_NAME}
    sensor_host_main.cpp

)
target_include_directories(${TEST_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(${TEST_NAME} PUBLIC PICO_CXX_DISABLE_ALLOCATION_OVERRIDES)

target_compile_definitions(${TEST_NAME} PUBLIC
CUSTOM_SLEEP_MS=sleep_ms)
# pull in common dependencies
target_link_libraries(${TEST_NAME} PUBLIC
    CppUTest
    CppUTestExt
    pico_stdlib
    sensors
)

# create map/bin/hex file etc.
pico_add_extra_outputs(${TEST_NAME})
pico_enable_stdio_usb(${TEST_NAME} 1)
pico_enable_stdio_uart(${TEST_NAME} 1)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--print-memory-usage")
# add url via pico_set_program_url

