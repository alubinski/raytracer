
enable_testing()
file(GLOB TEST_SOURCES src/*.cpp)

add_executable(raytracer_tests ${TEST_SOURCES})
target_link_libraries(raytracer_tests PRIVATE core Catch2::Catch2)
target_include_directories(raytracer_tests PRIVATE ${CMAKE_SOURCE_DIR}/core/include)

add_test(NAME RaytracerTests COMMAND raytracer_tests)
