#include "connection_handler.hpp"
#include "messaging.hpp"

namespace ucp {

  
  void connection_handler::operator()() {
    logger.debug("started connection handler");
    // make a copy of the shared pointer, this thread will live
    // longer than inclosing class
    messaging endpoint(  socket_ptr_ );
    
    server_connection_state state = initial;

    try {
      while( true ) {
	string client_message;
	string error_message;

	switch( state ) {
	case initial :
	  endpoint.receive( client_message );
	  if( client_message == CLIENT_VERSION_MSG ) {
	    state = waiting_for_direction;
	    endpoint.send( OK_MSG );
	  } else { 
	    state = invalid;
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
	  // method here
	case client_receive :
	  // method here
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
