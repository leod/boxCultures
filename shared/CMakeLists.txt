include_directories (${Enet_INCLUDE_DIRS})

add_library (shared tuple_util.hh
                    input_log.hh
                    input_stream.hh
                    message.hh
                    message.cc)

add_dependencies (shared enet)

set(Shared_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/shared
    ${PROJECT_SOURCE_DIR}/lib/cereal-1.2.2/include
    CACHE INTERNAL "Shared include directory")
