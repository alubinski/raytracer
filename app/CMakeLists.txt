file(GLOB APP_DIRS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*)

foreach(app_dir ${APP_DIRS})
  if(IS_DIRECTORY ${app_dir})
    get_filename_component(app_name ${app_dir} NAME)

    # Collect all .cpp files in this app folder
    file(GLOB app_sources CONFIGURE_DEPENDS ${app_dir}/*.cpp)

    add_executable(${app_name} ${app_sources})
    target_link_libraries(${app_name} PRIVATE core)

    message(STATUS "Configured app: ${app_name}")
  endif()
endforeach()

# file(GLOB APP_SOURCES src/*.cpp)
#
# add_executable(raytracer_app ${APP_SOURCES})
# target_link_libraries(raytracer_app PRIVATE core)
# target_include_directories(raytracer_app PRIVATE ${CMAKE_SOURCE_DIR}/core/include)
