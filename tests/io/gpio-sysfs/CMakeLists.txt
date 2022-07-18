add_executable(io_pin-open-test pin-open-test.c)
target_link_libraries(io_pin-open-test io_gpio-sysfs)
add_test(io_pin-open-test io_pin-open-test)

add_executable(io_pin-close-test pin-close-test.c)
target_link_libraries(io_pin-close-test io_gpio-sysfs)
add_test(io_pin-close-test io_pin-close-test)

add_executable(io_pin-direction-set-test pin-direction-set-test.c)
target_link_libraries(io_pin-direction-set-test io_gpio-sysfs)
add_test(io_pin-direction-set-test io_pin-direction-set-test)

add_executable(io_pin-direction-get-test pin-direction-get-test.c)
target_link_libraries(io_pin-direction-get-test io_gpio-sysfs)
add_test(io_pin-direction-get-test io_pin-direction-get-test)

add_executable(io_pin-value-set-test pin-value-set-test.c)
target_link_libraries(io_pin-value-set-test io_gpio-sysfs)
add_test(io_pin-value-set-test io_pin-value-set-test)

add_executable(io_pin-value-get-test pin-value-get-test.c)
target_link_libraries(io_pin-value-get-test io_gpio-sysfs)
add_test(io_pin-value-get-test io_pin-value-get-test)

add_executable(io_pin-value-toggle-test pin-value-toggle-test.c)
target_link_libraries(io_pin-value-toggle-test io_gpio-sysfs)
add_test(io_pin-value-toggle-test io_pin-value-toggle-test)

add_executable(io_pin-edge-exists-test pin-edge-exists-test.c)
target_link_libraries(io_pin-edge-exists-test io_gpio-sysfs)
add_test(io_pin-edge-exists-test io_pin-edge-exists-test)

add_executable(io_pin-edge-set-test pin-edge-set-test.c)
target_link_libraries(io_pin-edge-set-test io_gpio-sysfs)
add_test(io_pin-edge-set-test io_pin-edge-set-test)

add_executable(io_pin-edge-get-test pin-edge-get-test.c)
target_link_libraries(io_pin-edge-get-test io_gpio-sysfs)
add_test(io_pin-edge-get-test io_pin-edge-get-test)

add_executable(io_pin-value-get-evented-test pin-value-get-evented-test.c)
target_link_libraries(io_pin-value-get-evented-test io_gpio-sysfs)
target_link_libraries(io_pin-value-get-evented-test pthread)
add_test(io_pin-value-get-evented-test io_pin-value-get-evented-test)
