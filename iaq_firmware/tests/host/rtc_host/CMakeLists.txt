add_executable(rtc_host_test
    rtc_host_test.cpp
)

target_link_libraries(rtc_host_test
    CppUTest
    rtc
    pico_stdlib
    hardware_rtc
)

pico_add_test(rtc_host_test)
