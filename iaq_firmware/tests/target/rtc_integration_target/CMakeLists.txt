cmake_minimum_required(VERSION 3.13)

set(TEST_NAME rtc_Integration_target_test)

add_executable(${TEST_NAME}
    rtc_target_test.cpp
)

target_compile_definitions(${TEST_NAME} PRIVATE CPPUTEST_MEM_LEAK_DETECTION_DISABLED
    PICO_CXX_DISABLE_ALLOCATION_OVERRIDES=1)

# pull in common dependencies
target_link_libraries(${TEST_NAME} pico_stdlib
    CppUTest
    CppUTestExt
    rtc
)

# create map/bin/hex file etc.
pico_add_extra_outputs(${TEST_NAME})
pico_enable_stdio_usb(${TEST_NAME} 1)
pico_enable_stdio_uart(${TEST_NAME} 1)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--print-memory-usage")
