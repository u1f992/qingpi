cmake_minimum_required(VERSION 3.10)

project(qingpi)
set(CMAKE_C_STANDARD 99)

include_directories(include)
add_library(qingpi STATIC 
            src/button.c
            src/hat.c
            src/slidepad.c
            src/touchscreen.c)

# Create test executable
add_executable(test test/test.c)
target_link_libraries(test qingpi)