#include "client.hpp"
#include "messaging.hpp"

using ucp::logger;

namespace ucp { 

  client::client( const po::variables_map& command_arguments )
    :command( command_arguments ) {
    logger.debug(  "create client" );
    
  }

  client::~client() {
    logger.debug( "destroy client" );
  }

  void client::run()  {
    logger.debug( "run client" );
    logger.debug( (format("Preparing to connect to %1% on port %2%") % command.get_host() %
		   command.get_port() ).str() );

    UDT::startup();
    
    addrinfo hints;
    addrinfo* peer;
    memset( &hints, 0, sizeof( addrinfo ));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
      
    UDTSOCKET fhandle = UDT::socket(hints.ai_family, hints.ai_socktype, hints.ai_protocol );
    int response = getaddrinfo( command.get_host().c_str(), 
				command.get_service().c_str(),
				&hints, &peer );
      
    if(response != 0 ) {
      throw std::runtime_error( (format("Client connection failed. getaddrinfo returned %1% %2% %3%") 
				 % response 
				 % __FILE__ 
				 % __LINE__ ).str() );
    }

    response = UDT::connect( fhandle, peer->ai_addr, peer->ai_addrlen );
    
    if( UDT::ERROR == response ) {
      throw std::runtime_error( (format("%1% : %2% %3%") 
				 % UDT::getlasterror().getErrorMessage() 
				 % __FILE__
				 % __LINE__).str() );
    }

    // ok we're all connected up so lets talk to the server
    talk_to_server( fhandle );

    UDT::close( fhandle );
    UDT::cleanup();
    
  }

  void client::talk_to_server( UDTSOCKET socket ) {
  }

}
