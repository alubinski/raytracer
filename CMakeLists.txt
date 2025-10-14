
cmake_minimum_required(3.16)

project(raytracer)

include(cmake/import_catch2.cmake)

add_subdirectory(src)
add_subdirectory(tests)

