cmake_minimum_required(VERSION 3.2)

project(librasdk)

include_directories(include)

enable_testing()

add_subdirectory(source)
add_subdirectory(tests)