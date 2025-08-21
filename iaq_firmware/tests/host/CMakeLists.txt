# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(TESTS OFF CACHE BOOL "Disable tests" FORCE)

project(blink_app C CXX ASM)



# Add executable. Default name is the project name, version 0.1

add_executable(blink_app
    blink_app.c
)
target_link_libraries(blink_app
    blink
)
add_subdirectory(${IOT_FIRMWARE_ROOT_DIR}/tests/third_party/cpputest ${CMAKE_BINARY_DIR}/third_party/cpputest)


add_subdirectory(${IOT_FIRMWARE_ROOT_DIR}/src ${CMAKE_BINARY_DIR}/src)