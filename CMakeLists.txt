cmake_minimum_required(VERSION 3.1)
project(GameOfLife)

set(CMAKE_CXX_STANDARD 11)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
file(GLOB SOURCES src/*)

add_executable(main ${SOURCES})
target_link_libraries(main ${OPENGL_gl_LIBRARY} glfw)
target_include_directories(main PRIVATE src)
