#include "connection_handler.hpp"
#include "messaging.hpp"
#include "states.hpp"

namespace ucp {

  connection_handler::connection_handler( UDTSOCKET socket )
    :socket_(socket) {
    logger.debug( (format("Connected socket %1%") % socket).str() );
  }

  connection_handler::~connection_handler() {
   
  }

  void connection_handler::server_send_file( messaging endpoint ) {
    logger.debug( "server send file" );
    send::txfer_state state = send::wait_for_req_file;
    string client_message ;
    string error_message;
    string requested_file;

    while(true) {
      switch( state ) {
      case send::wait_for_req_file :
	endpoint.receive( requested_file );
	endpoint.send( OK_MSG );
	state = send::wait_for_file_size_req;
	break;
      case send::wait_for_file_size_req :
	endpoint.receive( client_message );
	logger.debug("get file size request");
	if( client_message == FILE_SIZE_REQ ) {
	  try {
	    endpoint.send( get_file_size( requested_file ) );
	    logger.debug("sent file size");
	  } catch( const std::exception& e ) {
	    error_message = e.what();
	    logger.warn( (format("file size request failed. Msg = %1%. %2% %3%") % error_message % __FILE__ % __LINE__ ).str() );
	    
	  }
	  state = send::wait_for_send_ack;
	} else {
	  logger.warn( (format("Expected %1% got %2% at %3% %4%") % FILE_SIZE_REQ % 
			client_message % __FILE__ % __LINE__ ).str() );
	  state = send::term;
	}		       
	break;
      case send::wait_for_send_ack :
	endpoint.receive(client_message );
	if( client_message == OK_MSG ) {
	  endpoint.send_file( requested_file );
	} else {
	  logger.warn((format("Unexpected response %1% at %2% %3%") %
		       client_message % __FILE__ % __LINE__ ).str() );	  
	}
	state = send::term;
	break;
      case send::error :
	endpoint.receive( client_message );
	if( client_message == OK_MSG ) {
	  endpoint.send( error_message );	  
	}
	state = send::term;
	break;
      case send::term :
	logger.debug( "get send term" );
	return;
	
      }
    }
    
  }

  void connection_handler::server_receive_file( messaging endpoint ) {
    logger.debug( "server receive file" );
    string file_name;
    int_t file_size = 0;
    recv::txfer_state state = recv::send_file_name;

    while( true ) {
      switch( state ) {
      case recv::send_file_name :
	endpoint.receive( file_name );
	state = recv::send_file_ack;
	break;
      case recv::send_file_ack :
	endpoint.send( OK_MSG );
	state = recv::wait_for_file_size;
	break;
      case recv::wait_for_file_size :
	endpoint.receive( &file_size );
	state = recv::ack_file_size;
	break;
      case recv::ack_file_size :
	endpoint.send( OK_MSG );
	state = recv::receive_file;
	break;
      case recv::receive_file :
	endpoint.receive_file( file_name, file_size );
	state = recv::term;
	break;
      case recv::term :
	return;
      }
    }
    
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
