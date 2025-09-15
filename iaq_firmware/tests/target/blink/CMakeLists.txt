# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(blink_test C CXX ASM)

add_executable(blink_test
    blink.cpp
    # IdleMemory.c
    hw_config.c
)
target_include_directories(blink_test 
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})    
# pull in common dependencies
target_link_libraries(blink_test pico_stdlib
    CppUTest
    CppUTestExt
    wifi
    FreeRTOS-Kernel-Heap4 # FreeRTOS kernel and dynamic heap
    FreeRTOS-Kernel
    FreeRTOS+FAT+CLI)

if(PICO_CYW43_SUPPORTED)
    target_link_libraries(blink_test pico_cyw43_arch_none)
endif()

# create map/bin/hex file etc.
pico_add_extra_outputs(blink_test)
pico_enable_stdio_usb(blink_test 1)
pico_enable_stdio_uart(blink_test 1)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--print-memory-usage")
# add url via pico_set_program_url
