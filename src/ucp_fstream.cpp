#include "ucp_fstream.hpp"

namespace ucp { 


  ucp_fstream::ucp_fstream( const string& filename, ios_base::openmode mode, shared_ptr< encryption_service > encryptor )
    :fstream( filename.c_str(), mode ), encryptor_(encryptor) {
  }

  ucp_fstream::~ucp_fstream() {
  }

  ostream& ucp_fstream::write( const char* buffer, streamsize sz ) {
    char plain_text[sz];
    encryptor_->decrypt( buffer, plain_text, sz );
    fstream::write( plain_text, sz );
    
    return *this;
  }

  istream& ucp_fstream::read( char* buffer, streamsize sz ) {
    char raw_buffer[ sz ];
    fstream::read( raw_buffer, sz );
    encryptor_->encrypt( raw_buffer, buffer, sz );

    return *this;
  } 

}
