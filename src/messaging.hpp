#ifndef __MESSAGING_HPP__
#define __MESSAGING_HPP__

#include "application.hpp"
#include "messages.hpp"
#include "encryption_service.hpp"

namespace ucp {
  enum messaging_role {
    client_role,
    server_role
  };

  class messaging {
    shared_ptr<UDTSOCKET> socket_;
    shared_ptr<encryption_service> encryptor_;

    static const unsigned int protocol_buffer_size = 2048;
    
    char protocol_buffer[protocol_buffer_size ];
    messaging_role role_;
    

    void trace_send( const string& msg );
    void trace_recv( const string& msg );
    void trace_send( int_t msg );
    void trace_recv( int_t msg );
  public:
    messaging( shared_ptr<UDTSOCKET> socket, messaging_role r ) 
      :socket_(socket), role_(r) {}
    virtual ~messaging();
    void send( const string& msg ) ;
    void send( int_t msg );
    void receive( string& buffer );
    void receive( int_t* msg );
    void close() ;
    void send_file( const string& file_name );     
    void receive_file( const string& file_name, int_t file_size );
    void enable_encryption( const string& secret_file_name ); 
  };

}


#endif
