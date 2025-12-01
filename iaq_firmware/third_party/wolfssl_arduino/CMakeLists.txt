
file(GLOB WOLFSSL_ARDUINO_SRC src/src/*.c src/wolfcrypt/src/*c src/wolfssl/*.c src/wolfssl-arduino.cpp)

add_library(wolfssl_arduino ${WOLFSSL_ARDUINO_SRC})
target_include_directories(wolfssl_arduino PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/wolfssl
)

target_compile_definitions(wolfssl_arduino PUBLIC
    ARDUINO
    WOLFSSL_USER_SETTINGS
)
