cmake_minimum_required(VERSION 3.1)
project(GameOfLife)


set(GLAD_GL "src/glad/gl.h"
            "src/glad_gl.c")

set(CMAKE_CXX_STANDARD 11) # no explicit compiler flags if possible
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
file(GLOB SOURCES src/*.cpp)

add_executable(main ${SOURCES} ${GLAD_GL})
target_link_libraries(main ${OPENGL_gl_LIBRARY} glfw)
target_include_directories(main PRIVATE src)
