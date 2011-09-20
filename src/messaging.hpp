#ifndef __MESSAGING_HPP__
#define __MESSAGING_HPP__

#include "application.hpp"

namespace ucp {
  class messaging {
    shared_ptr<UDTSOCKET> socket_;
    static const unsigned int protocol_buffer_size = 2048;
    
    char protocol_buffer[protocol_buffer_size ];
    
  public:
    messaging( shared_ptr<UDTSOCKET> socket ) 
      :socket_(socket) {}
    virtual ~messaging();
    void send( const string& msg ) ;
    void send( int_t msg );
    void receive( string& buffer );
    void receive( int_t* msg );
    void close() ;
     

  };

  static const char* CLIENT_VERSION_MSG = "UCP_CLIENT_V_0_1";
  static const char* OK_MSG = "OK";
  static const char* CLIENT_SEND_MSG = "CLIENT_SEND";
  static const char* CLIENT_RECEIVE_MSG = "CLIENT_RECEIVE";
  static const char* ERROR_MSG = "ERR";
  static const char* GOODBYE_MSG = "GOODBYE" ;

}


#endif
