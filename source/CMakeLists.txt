add_subdirectory(io)
add_library(rasdk core.c)
target_link_libraries(rasdk raio)
