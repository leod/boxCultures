include_directories (${Enet_INCLUDE_DIRS} ${Common_INCLUDE_DIRS})

add_executable (run_server Server.cc)

target_link_libraries (run_server enet winmm ws2_32 common)

add_dependencies (run_server enet common)