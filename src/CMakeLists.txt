include_directories(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(engine)
include_directories(engine/techniques)
include_directories(engine/map)
include_directories(engine/util)

file(GLOB_RECURSE derpus_SOURCES engine/*.cpp engine/*.h)

add_library(derpus SHARED ${derpus_SOURCES})

add_subdirectory(tests)
