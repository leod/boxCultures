cmake_minimum_required (VERSION 2.8.11)
project (boxCultures)

add_definitions (--std=c++11)

add_subdirectory (server)
add_subdirectory (game)
add_subdirectory (common)
add_subdirectory (lib/enet-1.3.13)
