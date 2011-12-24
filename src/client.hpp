#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include "application.hpp"
#include "copy_command.hpp"
#include "messaging.hpp"

namespace ucp {
  class client : public application {
    enum client_state {
      initial,
      hello_ack,
      error_msg,
      direction_ack,
      goodbye,
      term
    };

    friend shared_ptr<application> get_application( const po::variables_map& );
    
    copy_command command;

 
    

    client(const po::variables_map& );
    void talk_to_server( UDTSOCKET socket );
    void send_file( messaging& endpoint );
    void receive_file( messaging& endpoint );
    void secure_session();
  public:
    virtual ~client();
    virtual void run(); 
    virtual void signal(int) {}

  };
}

#endif
