include_directories (${Enet_INCLUDE_DIRS})

add_library (common BitStream.cc BitStream.hh Defs.cc Defs.hh GameSettings.cc GameSettings.hh Message.cc Message.hh Order.cc Order.hh)

#target_link_libraries (run_server enet)

add_dependencies (run_server enet)

set(Common_INCLUDE_DIRS
  ${PROJECT_SOURCE_DIR}/common
  CACHE INTERNAL "Common include directory")