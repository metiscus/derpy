cmake_minimum_required(VERSION 2.8)

project(OpenGlTest C CXX)
set(CMAKE_BUILD_TYPE Debug)

find_package(OpenGL)
if(OPENGL_FOUND)
	if(APPLE)
		set(CMAKE_CXX_LINK_FLAGS "-framework OpenGL -framework GLUT")
	endif(APPLE)
	
	include_directories(${OPENGL_INCLUDE_DIR})
endif(OPENGL_FOUND)

include_directories(extern/glfw-3.0.4/include)
include_directories(extern/glew-1.11.0/include)
include_directories(extern/glm)
add_subdirectory(extern)
add_subdirectory(src)
