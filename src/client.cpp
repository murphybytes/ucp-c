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
    client_state state = initial;
    messaging endpoint( shared_ptr<UDTSOCKET>( new UDTSOCKET(socket) ), client_role );
    string server_response;

    while( true ) {
      switch( state ) {
      case initial :
	endpoint.send( CLIENT_HELLO_MSG );
	state = hello_ack;
	break;
      case hello_ack :
	endpoint.receive( server_response );
	if( server_response == OK_MSG ) {
	  direction_t direction = command.get_direction() ;
	  assert( none != direction );
	  if( from_remote == direction ) {
	    endpoint.send( CLIENT_RECEIVE_MSG );
	    state = receive_ack;
	  } else { // to remote
	    endpoint.send( CLIENT_SEND_MSG );
	    state = send_ack;
	  }
	}

	if( server_response == ERROR_MSG ) {
	  state = error_msg;
	}
	break;
      case error_msg :
	endpoint.receive( server_response );
	logger.error( server_response );
	state = goodbye;
	break;
      case goodbye :
	endpoint.send( GOODBYE_MSG );
	state = term;
	break;
      case term :
	return;
      } // switch

    } // while
  }

}
