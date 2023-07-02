add_executable(firmware
    main.c

    button.c
    slidepad.c

    queue.c
    touchscreen.c

    router.c
    )

pico_enable_stdio_usb(firmware 1)
pico_enable_stdio_uart(firmware 0)

pico_add_extra_outputs(firmware)

target_link_libraries(firmware pico_stdlib hardware_adc)