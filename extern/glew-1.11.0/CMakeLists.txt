include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_library(glew SHARED src/glew.c)
target_link_libraries(glew ${OPENGL_LIBRARIES})