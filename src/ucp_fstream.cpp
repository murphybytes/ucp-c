#include "ucp_fstream.hpp"

namespace ucp { 


  fstream::fstream( const string& filename, ios_base::openmode mode, shared_ptr< encryption_service > encryptor )
    :std::fstream( filename.c_str(), mode ), encryptor_(encryptor) {
  }

  fstream::~fstream() {
  }

  ostream& fstream::write( const char* buffer, streamsize sz ) {
    char plain_text[sz];
    encryptor_->decrypt( buffer, plain_text, sz );
    std::fstream::write( plain_text, sz );
    
    return *this;
  }

  istream& fstream::read( char* buffer, streamsize sz ) {
    char raw_buffer[ sz ];
    std::fstream::read( raw_buffer, sz );
    
    encryptor_->encrypt( raw_buffer, buffer, gcount() );

    return *this;
  } 

}
