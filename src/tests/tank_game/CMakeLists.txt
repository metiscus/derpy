#map_physics test

include_directories(common)

set(COMMON_SRC common/Network.cpp common/TankMap.cpp)

set(SERVER_SRC 
	server/server.cpp
	server/GameSession.cpp
)

add_executable(tankServer ${SERVER_SRC} ${COMMON_SRC})

add_executable(tankClient client/client.cpp ${COMMON_SRC})

add_dependencies(tankClient derpus)
add_dependencies(tankServer derpus)

target_link_libraries(tankClient derpus glfw glew Box2D ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES})
target_link_libraries(tankServer derpus glfw glew Box2D ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES})