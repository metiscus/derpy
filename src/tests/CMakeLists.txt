#data files
file(COPY data/derp.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)
file(COPY data/crate.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)
file(COPY data/stonewall.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)
file(COPY data/boards.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data)
file(COPY data/map/tilesheet.gif DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/map)
file(COPY data/map/untitled.tmx DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/map)

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

#map_parse test
add_executable(map_parse map_parse/main.cpp)
add_dependencies(map_parse derpus)
target_link_libraries(map_parse derpus glfw glew ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES})

#map_physics test
add_executable(map_physics map_physics/main.cpp)
add_dependencies(map_physics derpus)
target_link_libraries(map_physics derpus glfw glew Box2D ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES})

add_subdirectory(dynamic_map)