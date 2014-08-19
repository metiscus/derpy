include_directories(${CMAKE_CURRENT_SOURCE_DIR})

file(GLOB ogltest_SOURCES *.cpp *.h)

add_executable( ogltest ${ogltest_SOURCES} )

target_link_libraries(ogltest glfw glew ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES})