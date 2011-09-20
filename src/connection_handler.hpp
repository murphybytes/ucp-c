#ifndef __CONNECTION_HANDLER_HPP__
#define __CONNECTION_HANDLER_HPP__


#include "application.hpp"

namespace ucp {

  class connection_handler {

    enum server_connection_state {
      initial,
      invalid,
      waiting_for_direction,
      client_expecting_ack,
      client_send,
      client_receive,
      success,
      error,
      term
    };


    shared_ptr<UDTSOCKET> socket_ptr_;
  public:
    connection_handler( shared_ptr<UDTSOCKET> socket_ptr ) 
      :socket_ptr_(socket_ptr){}
    virtual ~connection_handler() {}
    void operator()() ;

  };
} 



#endif
