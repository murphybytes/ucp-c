
#include <iostream>
#include <boost/test/minimal.hpp>
#include "copy_command.hpp"
#include "encryption_service.hpp"

int test_main( int argc, char* argv[] ) {

  string lcommand = "foo";
  string rcommand = "user@localhost:8080:~/bar";
  ucp::copy_command command( lcommand, rcommand );
  BOOST_CHECK( command.contains_remote_information( rcommand ) );
  BOOST_CHECK( !command.contains_remote_information( lcommand ) );
  rcommand = "user@:";
  BOOST_CHECK( !command.contains_remote_information( rcommand ) );
  rcommand = "user@my.host.com:80:" ;
  BOOST_CHECK( command.contains_remote_information( rcommand ) );
  rcommand = "user@my.host.com:/foo/bar/.";
  BOOST_CHECK( command.contains_remote_information( rcommand ));
  BOOST_CHECK( command.get_direction() != ucp::none );
  BOOST_CHECK( command.get_direction() == ucp::to_remote );
  BOOST_CHECK( command.get_host() == "localhost" );
  BOOST_CHECK( command.get_port() == 8080 );
  BOOST_CHECK( command.get_user() == "user" );
  BOOST_CHECK( command.get_target() == "~/bar" );
  BOOST_CHECK( command.get_source() == "foo" );
  command = ucp::copy_command( "abcd@eee.fff.com:", "xyz" );
  BOOST_CHECK( command.get_port() == 9090 );
  BOOST_CHECK( command.get_host() == "eee.fff.com" );
  BOOST_CHECK( command.get_source() == "" );
  BOOST_CHECK( command.get_target() == "xyz" );

  BOOST_CHECK( 332 == ucp::get_file_size("../data/testfile") );

  ucp::encryption_service encryption_service;
  byte_string shared_secret;
  
  encryption_service.generate_shared_secret( shared_secret );
  std::cout << "Length " << shared_secret.length() << std::endl;
  BOOST_CHECK( shared_secret.length() == 16 );

  return 0;

}



