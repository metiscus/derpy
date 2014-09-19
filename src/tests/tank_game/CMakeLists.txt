#map_physics test

include_directories(common)

add_executable(dynamic_map_server server/server.cpp common/TankMap.cpp)

add_executable(dynamic_map client/client.cpp common/TankMap.cpp)

add_dependencies(dynamic_map derpus)

target_link_libraries(dynamic_map derpus glfw glew Box2D ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES})