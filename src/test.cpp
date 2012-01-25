
#include <iostream>
#include <boost/test/minimal.hpp>
#include "copy_command.hpp"
#include "encryption_service.hpp"
#include "ucp_fstream.hpp"



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

  BOOST_CHECK( 4042 == ucp::get_file_size("../data/testfile") );

  ucp::encryption_service encryption_service;
  byte_string shared_secret;
  
  encryption_service.generate_shared_secret( shared_secret );
  std::cout << "Length " << shared_secret.length() << std::endl;
  std::cout << "content " << shared_secret.c_str()  << std::endl;

  std::cout << "key directory " << encryption_service.get_key_directory() << std::endl;
  std::cout << "shared secret length " << shared_secret.length() << std::endl;
  std::cout << "should be " << (AES::BLOCKSIZE + AES::DEFAULT_KEYLENGTH) << std::endl;
  BOOST_CHECK( shared_secret.length() == (AES::BLOCKSIZE + AES::DEFAULT_KEYLENGTH) );

  string file_name;
  encryption_service.write_shared_secret_to_file( shared_secret, file_name );
  byte_string read_from_file;
  encryption_service.read_shared_secret_from_file( file_name, read_from_file );
  std::cout << "read " << read_from_file.c_str() << std::endl;
  BOOST_CHECK( shared_secret == read_from_file );
 
  bool success = true ;
  try {
    encryption_service.send_shared_secret_to_remote_host( "dummy", "localhost", file_name );
  } catch( const std::runtime_error& e ) {
    success = false;
    std::cout << "Exception " << e.what() << std::endl;
  }
  BOOST_CHECK( success );

  ucp::encryption_service es( file_name ) ;
  string original = "red fox jumped over the fence";
  string cipher ;

  es.encrypt( original, cipher ) ;
  std::cout << "Cipher of '" <<  original << "'" << std::endl;
  es.pretty_print( cipher ) ;
  std::cout  << std::endl;
  BOOST_CHECK( original != cipher );
  string decrypted;
  es.decrypt( cipher, decrypted );
  std::cout << "decrypted > " << decrypted << std::endl;
  BOOST_CHECK( original == decrypted );

  original = "Four score and seven years ago our forefathers brought forst bla blab bla";
  cipher.clear();
  shared_secret.clear();
  es.generate_shared_secret( shared_secret );
  byte_string key = es.get_key_from_shared_secret( shared_secret );
  BOOST_CHECK( key.length() == AES::DEFAULT_KEYLENGTH );
  byte_string iv = es.get_iv_from_shared_secret( shared_secret );
  BOOST_CHECK( iv.length() == AES::BLOCKSIZE );
  ucp::encryption_service es1( shared_secret );
  es1.encrypt( original, cipher );
  decrypted.clear();
  es1.decrypt( cipher,decrypted);
  BOOST_CHECK( original == decrypted );

  std::cout << "orginal " << original << std::endl;
  std::cout << "decrypt " << decrypted << std::endl;
  shared_ptr<ucp::encryption_service> encryptor_ptr = shared_ptr<ucp::encryption_service>(new ucp::encryption_service( shared_secret ) );
  ucp::fstream fin(string("../README"), std::ios::in | std::ios::binary,  encryptor_ptr );
  ucp::fstream fout( string("README.TEMP"), std::ios::binary | ios::trunc | ios::out, encryptor_ptr );
  while( fin.good() ) {
    const size_t buff_size = 64;
    char buffer[buff_size];
    fin.read( buffer, buff_size );
    fout.write( buffer, fin.gcount() );
  }
  fin.close();
  fout.close();

  return 0;

}



