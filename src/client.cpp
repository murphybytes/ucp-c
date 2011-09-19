#include "client.hpp"


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
    
    try {
      addrinfo hints;
      addrinfo* res;
      memset( &hints, 0, sizeof( addrinfo ));
      hints.ai_flags = AI_PASSIVE;
      hints.ai_family = AF_INET;
      hints.ai_socktype = SOCK_STREAM;
      
      UDTSOCKET fhandle = UDT::socket(hints.ai_family, hints.ai_socktype, hints.ai_protocol );
      
    } catch( ... ) {
      UDT::cleanup();
    }
    
  }

}
