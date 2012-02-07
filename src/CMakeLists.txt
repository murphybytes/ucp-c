cmake_minimum_required ( VERSION 2.8 )
project (ucp)
set (ucp_VERSION_MAJOR 0)
set (ucp_VERSION_MINOR 2)
set( CMAKE_CXX_FLAGS "-g -O2" )

configure_file (
  "${PROJECT_SOURCE_DIR}/ucp_config.h.in"
  "${PROJECT_BINARY_DIR}/ucp_config.h"
)

link_directories( 
"${PROJECT_BINARY_DIR}/../deps/UDT4/src"
)

include_directories(
"${PROJECT_BINARY_DIR}"
"${PROJECT_BINARY_DIR}/../deps/UDT4/src"
)

add_library( ucp_core SHARED 
  application.cpp 
  listener.cpp
  client.cpp
  copy_command.cpp
  listen_thread.cpp
  connection_handler.cpp
  messaging.cpp
  encryption_service.cpp
  ucp_fstream.cpp
  user.cpp
  log.cpp
)

target_link_libraries( ucp_core 
boost_program_options
boost_regex
boost_signals 
boost_thread
boost_filesystem
boost_system
crypto++
udt
)


add_executable( ucp main.cpp )

target_link_libraries( ucp ucp_core )


add_executable( test  test.cpp )


target_link_libraries( test ucp_core )




