#include "connection_handler.hpp"
#include "messaging.hpp"

namespace ucp {

  connection_handler::connection_handler( UDTSOCKET socket )
    :socket_(socket) {
    logger.debug( (format("Connected socket %1%") % socket).str() );
  }

  connection_handler::~connection_handler() {
   
  }

  void connection_handler::server_send_file( messaging endpoint ) {
    logger.debug( "server send file" );
  }

  void connection_handler::server_receive_file( messaging endpoint ) {
    logger.debug( "server receive file" );
  }

  void connection_handler::operator()() {
    logger.debug("started connection handler");
    // make a copy of the shared pointer, this thread will live
    // longer than inclosing class
    messaging endpoint(  shared_ptr<UDTSOCKET>( new UDTSOCKET(socket_) ), server_role  ); 
    
    server_connection_state state = initial;

    try {
      while( true ) {
	string client_message;
	string error_message;

	switch( state ) {
	case initial :
	  endpoint.receive( client_message );
	  if( client_message == CLIENT_HELLO_MSG ) {
	    state = waiting_for_direction;
	    endpoint.send( OK_MSG );
	  } else { 
	    endpoint.send( ERROR_MSG );
	    state = error;
	  }
	  break;
	case waiting_for_direction :
	  endpoint.receive( client_message );
	  if( client_message == CLIENT_SEND_MSG ) {
	    state = client_send;
	    endpoint.send( OK_MSG );
	  } else if ( client_message == CLIENT_RECEIVE_MSG ) {
	    state = client_receive;
	    endpoint.send( OK_MSG );
	  } else {
	    state = error;
	    error_message = (format("Could not understand message: %1%") % client_message ).str();
	    endpoint.send( ERROR_MSG );
	  }
	  break;
	case error :
	  endpoint.receive( client_message );
	  if( client_message == OK_MSG ) {
	    endpoint.send( error_message );
	    state = term;
	  }
	  break;
	    
	case client_send :
	  server_receive_file( endpoint );
	  state = term;
	  break;

	case client_receive :
	  server_send_file( endpoint );
	  state = term;
	  break;

	case term :
	  endpoint.receive( client_message );
	  if( client_message != GOODBYE_MSG ) {
	    logger.warn( (format("Expecting %1%, got %2%") % GOODBYE_MSG % client_message ).str() );	
	  }
	  return;
	case invalid :
	  logger.warn( "got connection from client I dont know how to deal with ... exiting" );
	  return;
	}
      }
    } catch( const std::exception& e ) {
      logger.error( (format("Exception: %1% %2% %3%") % e.what() % __FILE__ % __LINE__ ).str() );
    }

    endpoint.close();
      
  } 


}
