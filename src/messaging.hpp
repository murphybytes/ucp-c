#ifndef __MESSAGING_HPP__
#define __MESSAGING_HPP__

#include "application.hpp"

namespace ucp {
  class messaging {
    shared_ptr<UDTSOCKET> socket_;
    static const unsigned int protocol_buffer_size = 2048;
    char protocol_buffer[protocol_buffer_size];
    
  public:
    messaging( shared_ptr<UDTSOCKET> socket ) 
      :socket_(socket) {}
    virtual ~messaging() {}
    void send( const string& msg ) ;
    void send( const char* buffer, unsigned int length );
    void receive( string& buffer );
     

  };

}


#endif
