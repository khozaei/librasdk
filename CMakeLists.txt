cmake_minimum_required(VERSION 3.5)

project(librasdk)

include(AddGitSubmodule.cmake)

add_git_submodule(tools/cmocka)

include_directories(include)

enable_testing()

add_subdirectory(source)
add_subdirectory(tests)


