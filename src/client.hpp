#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include "application.hpp"
#include "copy_command.hpp"
#include "messaging.hpp"
#include "encryption_service.hpp"

namespace ucp {
  /** 
   *  \brief Client connection handler
   *
   *  Class handles client side communication with remote server.  
   */
  class client : public application {
    enum client_state {
      initial,
      hello_ack,
      user_send_ack,
      session_ack,
      error_msg,
      direction_ack,
      goodbye,
      term
    };

    friend shared_ptr<application> get_application( const po::variables_map& );
    
    copy_command command;
    
    string shared_secret_file_name_;
    byte_string shared_secret_;
    po::variables_map command_arguments_;
    

    client(const po::variables_map& );
    void talk_to_server( UDTSOCKET socket  ) ;
    void send_file( messaging& endpoint );
    void receive_file( messaging& endpoint );
    void secure_session() ;
    void delete_secret_file() ;
  public:
    virtual ~client();
    virtual void run(); 
    virtual void signal(int) {}

  };
}

#endif
