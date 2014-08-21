include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(engine)

file(GLOB derpus_SOURCES engine/*.cpp engine/*.h)

add_library(derpus SHARED ${derpus_SOURCES})

add_subdirectory(tests)
