#data files
file(COPY data/derp.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)
file(COPY data/crate.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)
file(COPY data/stonewall.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)
file(COPY data/boards.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)

#opengl_basic test
add_executable(opengl_basic opengl_basic/main.cpp)
add_dependencies(opengl_basic derpus)
target_link_libraries(opengl_basic derpus glfw glew ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES})

#opengl_rendertotexture test
add_executable(opengl_rendertotexture opengl_rendertotexture/main.cpp)
add_dependencies(opengl_rendertotexture derpus)
target_link_libraries(opengl_rendertotexture derpus glfw glew ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES})

#opengl_lighting test
add_executable(opengl_lighting opengl_lighting/main.cpp)
add_dependencies(opengl_lighting derpus)
target_link_libraries(opengl_lighting derpus glfw glew ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES})
