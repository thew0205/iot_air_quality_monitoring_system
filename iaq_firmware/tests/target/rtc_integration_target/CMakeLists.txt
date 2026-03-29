cmake_minimum_required(VERSION 3.13)

set(TEST_NAME rtc_integration_target_test)

add_executable(${TEST_NAME}
    rtc_target_test.cpp
)

# 1. Get the current month (format %m returns 01-12)
string(TIMESTAMP CURRENT_YEAR "%Y")
string(TIMESTAMP CURRENT_MONTH "%m")
string(TIMESTAMP CURRENT_DAY "%d")
string(TIMESTAMP CURRENT_HOUR "%H")
string(TIMESTAMP CURRENT_MIN "%M")
string(TIMESTAMP CURRENT_SEC "%S")


# 2. Optional: Remove the leading zero (to mimic %-m)
# This prevents C from accidentally interpreting '08' or '09' as invalid octal numbers
string(REGEX REPLACE "^0" "" CURRENT_YEAR_CLEAN "${CURRENT_YEAR}")
string(REGEX REPLACE "^0" "" CURRENT_MONTH_CLEAN "${CURRENT_MONTH}")
string(REGEX REPLACE "^0" "" CURRENT_DAY_CLEAN "${CURRENT_DAY}")
string(REGEX REPLACE "^0" "" CURRENT_HOUR_CLEAN "${CURRENT_HOUR}")
string(REGEX REPLACE "^0" "" CURRENT_MIN_CLEAN "${CURRENT_MIN}")
string(REGEX REPLACE "^0" "" CURRENT_SEC_CLEAN "${CURRENT_SEC}")

target_compile_definitions(${TEST_NAME} PRIVATE
    CPPUTEST_MEM_LEAK_DETECTION_DISABLED
    PICO_CXX_DISABLE_ALLOCATION_OVERRIDES=1
    COMPILE_YEAR=${CURRENT_YEAR_CLEAN}
    COMPILE_MONTH=${CURRENT_MONTH_CLEAN}
    COMPILE_DAY=${CURRENT_DAY_CLEAN}
    COMPILE_HOUR=${CURRENT_HOUR_CLEAN}
    COMPILE_MIN=${CURRENT_MIN_CLEAN}
    COMPILE_SEC=${CURRENT_SEC_CLEAN}
)



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
