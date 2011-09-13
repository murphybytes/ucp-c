cmake_minimum_required ( VERSION 2.8 )
project (ucp)
set (ucp_VERSION_MAJOR 0)
set (ucp_VERSION_MINOR 1)

configure_file (
  "${PROJECT_SOURCE_DIR}/ucp_config.h.in"
  "${PROJECT_BINARY_DIR}/ucp_config.h"
)


include_directories("${PROJECT_BINARY_DIR}")

add_executable( ucp main.cpp )
target_link_libraries( ucp boost_program_options )