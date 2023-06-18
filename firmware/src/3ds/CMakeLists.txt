set(LIBRARY_NAME 3ds)
set(SOURCES
    button.cpp
    slidepad.cpp
    touchscreen.cpp
)
add_library(${LIBRARY_NAME} ${SOURCES})

target_include_directories(firmware PRIVATE ${LIBRARY_NAME})