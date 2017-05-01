include_directories (${Enet_INCLUDE_DIRS} ${Shared_INCLUDE_DIRS})

add_executable (game main.cc)

IF (WIN32)
	target_link_libraries (game enet winmm ws2_32 shared)
ELSE ()
	target_link_libraries (game enet shared)
ENDIF ()

add_dependencies (game enet shared)
