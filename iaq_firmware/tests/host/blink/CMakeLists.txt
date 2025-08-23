# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

project(blink_test C CXX ASM)


# Add executable. Default name is the project name, version 0.1

add_executable(blink_test
    blink.cpp
)
target_link_libraries(blink_test PRIVATE
                            CppUTest
                            CppUTestExt
                            wifi
                            
)