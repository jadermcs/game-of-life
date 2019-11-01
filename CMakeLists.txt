cmake_minimum_required(VERSION 3.1)
project(GameOfThreads)

set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wall -lpthread")
file(GLOB SOURCES src/*)

add_executable(game_of_threads ${SOURCES})
target_link_libraries(game_of_threads)
