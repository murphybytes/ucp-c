#ifndef __UCP_FSTREAM_HPP__
#define __UCP_FSTREAM_HPP__

#include "application.hpp"
#include "encryption_service.hpp"


using std::ios;
using std::ios_base;
using std::istream;
using std::ostream;
using std::streamsize;

namespace ucp {
  /**
   * \brief Encapsulates AES encryption / decryption for files
   *
   * Extends std::fstream so we can seamlessly incorporate AES 
   * encryption into UDT library. 
   */
  class fstream : public std::fstream {
    shared_ptr< encryption_service > encryptor_;

  public:
    fstream( const string& , ios_base::openmode, shared_ptr< encryption_service > );
    virtual ~fstream();

    virtual istream& read( char* buffer, streamsize sz ); 
    virtual ostream& write( const char* buffer, streamsize sz );    
  };
}


#endif
