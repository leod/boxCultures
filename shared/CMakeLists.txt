include_directories (${Enet_INCLUDE_DIRS})

add_library (shared message.hh message.cc)

add_dependencies (shared enet)

set(Shared_INCLUDE_DIRS
    ${PROJECT_SOURCE_DIR}/shared
    CACHE INTERNAL "Shared include directory")
