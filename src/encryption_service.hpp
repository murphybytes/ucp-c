#ifndef __ENCRYPTION_SERVICE_HPP__
#define __ENCRYPTION_SERVICE_HPP__

#include "application.hpp"
#include <cryptopp/aes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/cryptlib.h>
#include <cryptopp/modes.h>
#include <cryptopp/base64.h>

using CryptoPP::StringSink;
using CryptoPP::Base64Encoder;
using CryptoPP::StreamTransformationFilter;
using CryptoPP::StringSource;

namespace ucp {

  typedef CryptoPP::CBC_Mode< CryptoPP::AES >::Encryption encryptor_t;
  typedef CryptoPP::CBC_Mode< CryptoPP::AES >::Decryption decryptor_t;
  
  class encryption_service {
    string key_directory;
    shared_ptr< encryptor_t > encryptor_;
    shared_ptr< decryptor_t > decryptor_;
  public:
    void generate_shared_secret( byte_string& shared_secret ) const;
    void write_shared_secret_to_file( const byte_string& shared_secret, string& file_name ) const ;
    void read_shared_secret_from_file( const string& file_name, byte_string& shared_secret ) const;
    void get_random_filename( string& file_name ) const;
    void send_shared_secret_to_remote_host( const string& remote_user, const string& remote_host, const string& file_name ) const;
    const string& get_key_directory() const { return key_directory; } 
    void encrypt( const string& plain_text, string& cipher ) const;
    void decrypt( const string& cipher, string& plain_text ) const ;
    encryption_service() ;
    encryption_service( const string& secret_file_name );
    encryption_service( const byte_string& shared_secret );
    virtual ~encryption_service() {
    }
  };

}


#endif
