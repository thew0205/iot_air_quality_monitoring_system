

set(WOLFSSL_ROOT_DIR ${IOT_FIRMWARE_ROOT_DIR}/third_party/wolfssl)
file(GLOB WOLFSSL_SRC
    ${WOLFSSL_ROOT_DIR}/src/*.c
    ${WOLFSSL_ROOT_DIR}/wolfcrypt/src/*.c
    ${WOLFSSL_ROOT_DIR}/wolfcrypt/src/port/rpi_pico/*.c
    ${WOLFSSL_ROOT_DIR}/wolfssl/*.c
)

add_library(wolfssl ${WOLFSSL_SRC})
target_include_directories(wolfssl PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/
    ${WOLFSSL_ROOT_DIR}/
    ${WOLFSSL_ROOT_DIR}/src
    ${IOT_FIRMWARE_ROOT_DIR}/include
)

target_compile_definitions(wolfssl PUBLIC
    WOLFSSL_USER_SETTINGS
    WOLFSSL_NO_SOCK
    WOLFSSL_USER_IO
    NO_ASN_TIME
    WOLFSSL_RPIPICO
)


target_link_libraries(wolfssl PRIVATE pico_rand)