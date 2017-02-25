include_directories (${Enet_INCLUDE_DIRS} ${Common_INCLUDE_DIRS})

add_executable (game Client.cc Client.hh Main.cc)

target_link_libraries (game enet winmm ws2_32 common)

add_dependencies (game enet common)