cmake_minimum_required(VERSION 3.1)
project(GameOfLife)

set(OpenGL_GL_PREFERENCE LEGACY)
set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -O3 -Wall -lpthread")
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
file(GLOB SOURCES src/*)

add_executable(main ${SOURCES})
target_link_libraries(main ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARIES} glfw)
