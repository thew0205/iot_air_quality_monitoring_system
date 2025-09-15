cmake_minimum_required(VERSION 3.13)

set(TEST_NAME freertos_host_test)

add_executable(${TEST_NAME}
    freertos_host_test.cpp
)
target_link_libraries(${TEST_NAME} PRIVATE
    freertos_common_test
)
