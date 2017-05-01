cmake_minimum_required (VERSION 2.8.11)
project (boxCultures)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory (server)
add_subdirectory (game)
add_subdirectory (shared)
add_subdirectory (lib/enet-1.3.13)
