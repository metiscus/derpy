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

if(CMAKE_COMPILER_IS_GNUCXX)
	# This enables valgrind to be happy
	set(CMAKE_CXX_FLAGS_DEBUG "-DGLIBCXX_FORCE_NEW -g")
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -std=c++0x -g")

endif(CMAKE_COMPILER_IS_GNUCXX)

include_directories(extern/glfw-3.0.4/include)
include_directories(extern/glew-1.11.0/include)
include_directories(extern/rapidxml-1.13)
include_directories(extern/glm)
include_directories(extern/Box2D)
add_subdirectory(extern)
add_subdirectory(src)
