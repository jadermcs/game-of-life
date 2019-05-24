cmake_minimum_required(VERSION 3.1)
project(GameOfLife)

set(CMAKE_CXX_STANDARD 11) # no explicit compiler flags if possible
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
file(GLOB SOURCES src/*.cpp)

add_executable(main ${SOURCES})
target_link_libraries(main ${OPENGL_gl_LIBRARY} glfw)
target_include_directories(main PRIVATE src)

# add_custom_command(TARGET LearnGLSL POST_BUILD
#     COMMAND ${CMAKE_COMMAND} -E copy_directory
#     ${CMAKE_SOURCE_DIR}/assets
#     ${PROJECT_BINARY_DIR}
#     COMMENT "Copy resources to build tree")
