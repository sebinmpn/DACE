cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(test_project C CXX ASM)
set(CAMKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(test
    main.c
    )

pico_enable_stdio_usb(test 1)
pico_enable_stdio_uart(test 1)

pico_add_extra_outputs(test)

target_link_libraries(test pico_stdlib ili9341 gfx max30102 mlx90614)

add_subdirectory(lib)
add_subdirectory(max)
add_subdirectory(mlx90614)
