cmake_minimum_required(VERSION 3.13)

set(TEST_NAME freertos_fat_target_test)

add_executable(${TEST_NAME}
    freertos_fat_target_test.cpp
    hw_config.c
)
target_include_directories(${TEST_NAME} 
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})    
# pull in common dependencies
target_link_libraries(${TEST_NAME} pico_stdlib
    CppUTest
    CppUTestExt
    FreeRTOS-Kernel-Heap4 # FreeRTOS kernel and dynamic heap
    FreeRTOS-Kernel
    FreeRTOS+FAT+CLI)

if(PICO_CYW43_SUPPORTED)
    target_link_libraries(${TEST_NAME} pico_cyw43_arch_none)
endif()

# create map/bin/hex file etc.
pico_add_extra_outputs(${TEST_NAME})
pico_enable_stdio_usb(${TEST_NAME} 1)
pico_enable_stdio_uart(${TEST_NAME} 1)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--print-memory-usage")
# add url via pico_set_program_url
