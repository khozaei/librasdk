cmake_minimum_required(VERSION 3.5)

project(librasdk)

include(AddGitSubmodule.cmake)

option(WITH_EXAMPLES "Build examples" OFF)
option(UNIT_TESTING "Build with unit testing" OFF)

add_git_submodule(tools/cmocka)

include_directories(include)

enable_testing()

add_subdirectory(source)
add_subdirectory(tests)
add_subdirectory(examples)

