cmake_minimum_required ( VERSION 2.8 )
project (ucp)
set (ucp_VERSION_MAJOR 0)
set (ucp_VERSION_MINOR 1)
set( CMAKE_CXX_FLAGS "-g" )

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

add_executable( ucp main.cpp
  application.cpp 
  listener.cpp
  client.cpp
  copy_command.cpp)

target_link_libraries( ucp 
boost_program_options
boost_regex 
udt
)

add_executable( test  test.cpp
  copy_command.cpp )

target_link_libraries( test
boost_program_options
boost_regex )