add_executable(firmware main.cpp)
add_subdirectory(3ds)

pico_enable_stdio_usb(firmware 1)
# pico_enable_stdio_uart(firmware 1)

pico_add_extra_outputs(firmware)

target_link_libraries(firmware pico_stdlib)