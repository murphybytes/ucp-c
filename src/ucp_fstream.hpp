#ifndef __UCP_FSTREAM_HPP__
#define __UCP_FSTREAM_HPP__

#include "application.hpp"
#include "encryption_service.hpp"

using std::fstream;
using std::ios;
using std::ios_base;
using std::istream;
using std::ostream;
using std::streamsize;

namespace ucp {


  class ucp_fstream : public std::fstream {
    shared_ptr< encryption_service > encryptor_;

  public:
    ucp_fstream( const string& , ios_base::openmode, shared_ptr< encryption_service > );
    virtual ~ucp_fstream();

    virtual istream& read( char* buffer, streamsize sz ); 
    virtual ostream& write( const char* buffer, streamsize sz );    
  };
}


#endif
