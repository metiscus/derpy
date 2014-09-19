#map_physics test

add_executable(dynamic_map_server server.cpp TankMap.cpp)
add_executable(dynamic_map main.cpp TankMap.cpp)
add_dependencies(dynamic_map derpus)
target_link_libraries(dynamic_map derpus glfw glew Box2D ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES})