file(GLOB_RECURSE CORE_SOURCES src/*.cpp)
file(GLOB_RECURSE CORE_HEADERS include/core/*.h)

add_library(core STATIC ${CORE_SOURCES} ${CORE_HEADERS})
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
