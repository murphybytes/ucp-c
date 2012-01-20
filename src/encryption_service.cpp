
#include "encryption_service.hpp"
#include <fstream>
#include <unistd.h>
#include <stdlib.h>

using CryptoPP::AutoSeededRandomPool;
using CryptoPP::Rijndael_Info;
using std::fstream;



namespace ucp {


  static const char* CHAR_MAPPING = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+-";

  encryption_service::encryption_service() {
    key_directory = (format("%1%/.ucp") % getenv( "HOME" )).str() ;
  }

  encryption_service::encryption_service( const string& secret_file_name ) {
    key_directory = (format("%1%/.ucp") % getenv( "HOME" )).str() ;
    byte_string shared_secret;     

    read_shared_secret_from_file( secret_file_name, shared_secret_ );
    byte_string key = get_key_from_shared_secret( shared_secret_ );
    byte_string iv = get_iv_from_shared_secret( shared_secret_ );
    encryptor_.SetKeyWithIV( key.data(), key.size(), iv.data() );
    decryptor_.SetKeyWithIV( key.data(), key.size(), iv.data() );
  }

  

  encryption_service::encryption_service( const byte_string& shared_secret ) 
    :shared_secret_(shared_secret) {
    byte_string key = get_key_from_shared_secret( shared_secret_ );
    byte_string iv = get_iv_from_shared_secret( shared_secret_ );
    encryptor_.SetKeyWithIV( key.data(), key.size(), iv.data() );
    decryptor_.SetKeyWithIV( key.data(), key.size(), iv.data() );
  }

  void encryption_service::encrypt( const string& plain_text, string& cipher ) {
    if( ! shared_secret_.empty()  ) {
      StringSource s( plain_text, true, 
		      new StreamTransformationFilter( encryptor_, 
						      new StringSink( cipher ) ));
    } else { 
      cipher = plain_text;
    }										 
												 
  }
  
  void encryption_service::decrypt( const string& cipher, string& plain_text )   {
    if( ! shared_secret_.empty()  ) {
      StringSource s( cipher, true, 
		      new StreamTransformationFilter( decryptor_,
						      new StringSink( plain_text ) ));
      // byte out[cipher.size()];
      // memcpy( out, cipher.data(), cipher.size() );
      // decryptor_.ProcessData( out,out, cipher.size() );
      // plain_text = (const char*)out;
    } else { 
      plain_text = cipher ;
    }
    
  }


  void encryption_service::pretty_print( const string& cipher ) {
    string encoded;
    StringSource source( cipher, true, new HexEncoder( new StringSink( encoded ) )  );
    cout << encoded ;
  }

  byte_string encryption_service::encode( const byte_string& value ) const {
    byte_string result ;
    for( byte_string::const_iterator ch = value.begin(); ch != value.end(); ++ch ) {
      result.push_back( CHAR_MAPPING[ *ch % 64 ] );
    }
    return result;
  }

  unsigned char* encryption_service::encode( unsigned char* buffer, size_t length ) const {
    for( size_t i = 0; i < length; ++i ) {
      buffer[i] = CHAR_MAPPING[ buffer[i] % 64 ] ;
    }
    return buffer;
  }

  void encryption_service::generate_shared_secret( byte_string& shared_secret ) const {
    shared_secret.clear();
    AutoSeededRandomPool generator;
    Rijndael_Info aes_params;
    unsigned char  key[ AES::DEFAULT_KEYLENGTH + 1]; key[AES::DEFAULT_KEYLENGTH] = 0;
    unsigned char iv[AES::BLOCKSIZE + 1]; iv[AES::BLOCKSIZE] = 0;
    generator.GenerateBlock( key, AES::DEFAULT_KEYLENGTH );
    generator.GenerateBlock( iv, AES::BLOCKSIZE );
    shared_secret.append( encode( key, AES::DEFAULT_KEYLENGTH ) );
    shared_secret.append( encode( iv, AES::BLOCKSIZE ) );
    
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
    
    fs::path ucp_hidden_path( key_directory );
    Rijndael_Info aes_params;
    if( !fs::exists( ucp_hidden_path ) ) {
      fs::create_directory( ucp_hidden_path ) ;
    }

    get_random_filename( file_name );
    ucp_hidden_path.append( file_name.begin(), file_name.end() );
    file_name = ucp_hidden_path.c_str();
    std::fstream stream( file_name.c_str(), std::fstream::out );
    stream.write( (char*)shared_secret.data(), shared_secret.size()  );
    stream.close();
    
    
  }

  void encryption_service::read_shared_secret_from_file( const string& file_name, byte_string& shared_secret ) const {
    Rijndael_Info aes_params;
    fs::path ucp_hidden_path( file_name );
    std::fstream stream( ucp_hidden_path.c_str(), std::fstream::in );
    char buffer[ AES::DEFAULT_KEYLENGTH + AES::BLOCKSIZE + 1];
    buffer[ sizeof(buffer) ] = 0;
    stream.get( buffer, sizeof(buffer)  );
    stream.close();
    shared_secret.append( (unsigned char*)buffer );
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
