#include <cryptopp/aes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/cryptlib.h>
#include "encryption_service.hpp"
#include <fstream>
#include <unistd.h>
#include <stdlib.h>

using CryptoPP::AutoSeededRandomPool;
using CryptoPP::Rijndael_Info;
using std::fstream;



namespace ucp {

  static const char* UCP_DIRECTORY = "/home/jam/.ucp";
  static const char* CHAR_MAPPING = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

  void encryption_service::generate_shared_secret( byte_string& shared_secret ) const {
    AutoSeededRandomPool generator;
    Rijndael_Info aes_params;
    unsigned char  key[ aes_params.DEFAULT_KEYLENGTH ];
    generator.GenerateBlock( key, aes_params.DEFAULT_KEYLENGTH );
    for( int i = 0; i < aes_params.DEFAULT_KEYLENGTH; ++i ) {
      shared_secret.push_back( CHAR_MAPPING[ key[i] % 64 ] );      
    }
    
  }

  void encryption_service::get_random_filename( string& file_name ) const {
    AutoSeededRandomPool generator;
    const int buffer_len = 24;
    file_name.clear();
    unsigned char buffer[buffer_len];
    generator.GenerateBlock( buffer, buffer_len );
    for( int i = 0; i < buffer_len; ++i ) {
      file_name.push_back( CHAR_MAPPING[ buffer[i] % 64 ] );
    }
    
  }

  void encryption_service::write_shared_secret_to_file( const byte_string& shared_secret, string& file_name ) const  {
    fs::path ucp_hidden_path(  UCP_DIRECTORY );
    Rijndael_Info aes_params;
    if( !fs::exists( ucp_hidden_path ) ) {
      fs::create_directory( ucp_hidden_path ) ;
    }

    get_random_filename( file_name );
    ucp_hidden_path.append( file_name.begin(), file_name.end() );
    file_name = ucp_hidden_path.c_str();
    std::fstream stream( file_name.c_str(), std::fstream::out );
    stream.write( (char*)shared_secret.data(), aes_params.DEFAULT_KEYLENGTH  );
    stream.close();
    
    
  }

  void encryption_service::read_shared_secret_from_file( const string& file_name, byte_string& shared_secret ) const {
    Rijndael_Info aes_params;
    fs::path ucp_hidden_path( file_name );
    std::fstream stream( ucp_hidden_path.c_str(), std::fstream::in );
    char buffer[ aes_params.DEFAULT_KEYLENGTH + 1];
    buffer[ aes_params.DEFAULT_KEYLENGTH ] = 0;
    stream.get( buffer, aes_params.DEFAULT_KEYLENGTH + 1  );
    stream.close();
    shared_secret.append( (unsigned char*)buffer, aes_params.DEFAULT_KEYLENGTH );
  }


  void encryption_service::send_shared_secret_to_remote_host( const string& remote_user, const string& remote_host, const string& file_name ) const {
    std::stringstream command ;
    command << "scp " << file_name << " "  << remote_user << "@" << remote_host << ":.ucp/.";
    int result = system( command.str().c_str() );

    if( result ) {
      throw std::runtime_error( (format("Failed to copy key to remote %1% %2% %3%") %
				 command.str() % __FILE__ % __LINE__ ).str() );
    }
      
 
    
  }


}
