add_executable(test_io_open-close-chipinfo open-close-chipinfo.c)
target_link_libraries(test_io_open-close-chipinfo io_gpio-chardev)
add_test(test_io_open-close-chipinfo test_io_open-close-chipinfo)

add_executable(test_io_gpio-line-requst-and-info-get gpio-line-requst-and-info-get.c)
target_link_libraries(test_io_gpio-line-requst-and-info-get io_gpio-chardev)
add_test(test_io_gpio-line-requst-and-info-get test_io_gpio-line-requst-and-info-get)

add_executable(test_io_gpio-lines-config-set gpio-lines-config-set.c)
target_link_libraries(test_io_gpio-lines-config-set io_gpio-chardev)
add_test(test_io_gpio-lines-config-set test_io_gpio-lines-config-set)

add_executable(test_io_gpio-value-get-set gpio-value-get-set.c)
target_link_libraries(test_io_gpio-value-get-set io_gpio-chardev)
add_test(test_io_gpio-value-get-set test_io_gpio-value-get-set)