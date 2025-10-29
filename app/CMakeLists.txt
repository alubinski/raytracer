
file(GLOB APP_SOURCES src/*.cpp)

add_executable(raytracer_app ${APP_SOURCES})
target_link_libraries(raytracer_app PRIVATE core)
target_include_directories(raytracer_app PRIVATE ${CMAKE_SOURCE_DIR}/core/include)
