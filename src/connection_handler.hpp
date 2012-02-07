#ifndef __CONNECTION_HANDLER_HPP__
#define __CONNECTION_HANDLER_HPP__


#include "application.hpp"
#include "messaging.hpp"

namespace ucp {
  /** 
   *  \brief Handles connection with remote client.
   *
   *  Server side class that manages connection with a remote client.  Each connection 
   *  handler exists on it's own thread. 
   */
  class connection_handler {

    enum server_connection_state {
      initial,
      invalid,
      waiting_for_direction,
      waiting_for_secret_file, 
      waiting_for_user_name,
      client_expecting_ack,
      client_send,
      client_receive,
      success,
      error,
      term
    };


    UDTSOCKET socket_;
    string last_error_;

    void server_send_file( messaging endpoint );
    void server_receive_file( messaging endpoint );
    void delete_secret_file( const user& u, const string& secret_file_name );
  public:
    connection_handler( UDTSOCKET socket ); 
    connection_handler( const po::variables_map& );
    virtual ~connection_handler();
    const string& last_error() const { return last_error_; }
    void operator()(shared_ptr<std::fstream>, unsigned int) ;

  };
} 



#endif
