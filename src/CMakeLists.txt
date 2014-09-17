include_directories(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(engine)
include_directories(engine/techniques)
include_directories(engine/map)
include_directories(engine/util)

file(GLOB_RECURSE derpus_SOURCES engine/*.cpp engine/*.h)

add_library(derpus STATIC ${derpus_SOURCES})
target_link_libraries(derpus glfw glew ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES})

add_subdirectory(tests)
