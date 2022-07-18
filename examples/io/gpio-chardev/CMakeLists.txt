
add_executable(example_io_pins-value pins-value.c)
target_link_libraries(example_io_pins-value io_gpio-chardev)

add_executable(example_io_edge-detection edge-detection.c)
target_link_libraries(example_io_edge-detection io_gpio-chardev)

add_executable(example_io_get-line-info get-line-info.c)
target_link_libraries(example_io_get-line-info io_gpio-chardev)