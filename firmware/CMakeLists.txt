# P.32
cmake_minimum_required(VERSION 3.13)

include(cmake/pico_sdk_import.cmake)

project(firmware C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(firmware
    src/main.c
)

pico_enable_stdio_usb(firmware 1)
# pico_enable_stdio_uart(firmware 1)

pico_add_extra_outputs(firmware)

target_link_libraries(firmware pico_stdlib)