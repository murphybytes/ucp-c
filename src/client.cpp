#include "client.hpp"
#include "states.hpp"
#include <crypto++/osrng.h>

using ucp::logger;

#define THROW_ON_ERROR( x )     if( UDT::ERROR == x ) { \
      throw std::runtime_error( (format("%1% : %2% %3%")  \
                                 % UDT::getlasterror().getErrorMessage() \
                                 % __FILE__ \
                                 % __LINE__).str() ); \
    }


namespace ucp { 

  client::client( const po::variables_map& command_arguments )
    :command( command_arguments ), command_arguments_(command_arguments)
  {
    logger.debug(  "create client" );
    
  }

  client::~client() {
    logger.debug( "destroy client" );
  }
  
  /*
    void client::generate_keys() {
    AutoSeededRandomPool random_number_generator;

    }
  */
  // TODO: 
  // 1) Generate AES key
  // 2) Write AES key to .ucp/<pid>.<clienthostname>.key
  // 3) Fork process copy key file to server 
  // 4) wait until fork returns
  void client::secure_session() {
    ucp::encryption_service encryption_service ;

    encryption_service.generate_shared_secret( shared_secret_ );
    logger.debug( (format("Shared secret => %1%") % shared_secret_.c_str() ).str() );
    encryption_service.write_shared_secret_to_file( shared_secret_, shared_secret_file_name_ );
    logger.debug( (format("Shared secret file => %1%") % shared_secret_file_name_ ).str() );
    encryption_service.send_shared_secret_to_remote_host( command.get_user(), command.get_host(), shared_secret_file_name_ );
    
  
  }

  void client::run()  {
    logger.debug( "run client" );
    logger.debug( (format("Preparing to connect to %1% on port %2%") % command.get_host() %
                   command.get_port() ).str() );
    
    secure_session();
  
    UDT::startup();
    
    addrinfo hints;
    addrinfo* peer;
    memset( &hints, 0, sizeof( addrinfo ));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
      
    UDTSOCKET fhandle = UDT::socket(hints.ai_family, hints.ai_socktype, hints.ai_protocol );
    logger.debug((format("Client connect host: %1%, service: %2%") % command.get_host() % command.get_service() ).str() );
    int response = getaddrinfo( command.get_host().c_str(), 
                                command.get_service().c_str(),
                                &hints, &peer );
      
    if(response != 0 ) {
      throw std::runtime_error( (format("Client connection failed. getaddrinfo returned %1% %2% %3%") 
                                 % response 
                                 % __FILE__ 
                                 % __LINE__ ).str() );
    }


    const char* max_bandwidth = reinterpret_cast< const char* >( &command_arguments_["max-bandwidth"].as<int_t>() );
    THROW_ON_ERROR( UDT::setsockopt( fhandle, ucp::UNUSED, UDT_MAXBW, max_bandwidth, sizeof( int_t ) ) )

    THROW_ON_ERROR( UDT::connect( fhandle, peer->ai_addr, peer->ai_addrlen ) )
    
 
    logger.debug("client connected");
    // ok we're all connected up so lets talk to the server
    talk_to_server( fhandle );

    UDT::close( fhandle );
    UDT::cleanup();
    
  }

  void client::receive_file( messaging& endpoint ) {
    recv::txfer_state state = recv::send_file_name;
    int_t file_size = 0;
    string message;

    while(true) {
      switch( state ) {
      case recv::send_file_name :
	endpoint.send( command.get_source() ); 
	state = recv::send_file_ack;
	break;
      case recv::send_file_ack :
	endpoint.receive( message );
	if( OK_MSG == message ) {
	  state = recv::request_file_size;
	} else {
	  state = recv::error;
	}
	break;
      case recv::request_file_size :
	endpoint.send(FILE_SIZE_REQ);
	state = recv::receive_file_size;
	break;
      case recv::receive_file_size :
	logger.debug("getting file size");
	endpoint.receive( &file_size );
	if( ERROR_INT == file_size ) {
	  state = recv::error_ack;
	} else {
	  state = recv::ack_file_size;
	}
	break;
      case recv::ack_file_size :
	endpoint.send( OK_MSG );
	state = recv::receive_file;
	break;
      case recv::receive_file :
	endpoint.receive_file( command.get_target(), file_size );
	state = recv::term;
	break;
      case recv::error_ack :
	endpoint.send( OK_MSG );
	state = recv::error;
	break;
      case recv::error :
	logger.debug("error");
	state = recv::term;
	endpoint.receive( message );
	logger.error( message );
	break;
      case recv::term :
	return;
      }
    }
  }

  void client::send_file( messaging& endpoint ) {
    logger.debug("send file");
    send::txfer_state state = send::file_name;
    string server_response;

    while( true ) {
      switch( state ) {
      case send::file_name :
	endpoint.send( command.get_target() );
	state = send::file_name_ack;
	break;
      case send::file_name_ack :
	endpoint.receive( server_response );
	if( server_response == OK_MSG ) {
	  state = send::file_size ;
	} else {
	  state = send::error_ack;
	}
	break;
      case send::file_size :
	endpoint.send( get_file_size( command.get_source() ) );
	state = send::file_size_ack;
	break;
      case send::file_size_ack :
	endpoint.receive( server_response );
	if( server_response == OK_MSG ) {
	  state = send::file_send;	  
	} else {
	  state = send::error_ack;
	}
	break;
      case send::file_send :
	endpoint.send_file( command.get_source() );
	state = send::term;
	break;
      case send::error_ack :
	endpoint.send( OK_MSG );
	state = send::error;
	break;
      case send::error :
	endpoint.receive( server_response );
	logger.error( server_response );
	state = send::term;
      case send::term :
	logger.debug( "got term" );
	return;
      }
    }
  }
  
  

  void client::talk_to_server( UDTSOCKET socket )  {
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
	endpoint.receive( server_response ) ;
	if( server_response == OK_MSG ) {
	  endpoint.send( command.get_user() );
	  state = user_send_ack;
	} 
	if( server_response == ERROR_MSG ) {
	  state = error_msg; 
	}
	break;
      case user_send_ack :
	endpoint.receive( server_response ) ;
	if( server_response == OK_MSG ) {

	  endpoint.send( boost::filesystem3::path( shared_secret_file_name_ ).filename().string() );
	  state = session_ack;
	}

	if( server_response == ERROR_MSG ) {
	  state = error_msg;
	}
	break;

      case session_ack :
        endpoint.receive( server_response );
        if( server_response == OK_MSG ) {
	  endpoint.enable_encryption( shared_secret_file_name_ );
	  delete_secret_file( );
          direction_t direction = command.get_direction() ;
          assert( none != direction );
          if( from_remote == direction ) {
            endpoint.send( CLIENT_RECEIVE_MSG );
          } else { // to remote
            endpoint.send( CLIENT_SEND_MSG );
          }
          state = direction_ack;
        }

        if( server_response == ERROR_MSG ) {
          state = error_msg;
          endpoint.send( OK_MSG );
        }
        break;
      case direction_ack :
        endpoint.receive( server_response );
        if( OK_MSG == server_response ) {
          if( command.get_direction() == from_remote ) {
            receive_file( endpoint );
          } else {
            send_file( endpoint );
          }
        } else if( ERROR_MSG == server_response ) {
          endpoint.send( OK_MSG );
          state = error_msg;
        } 
	state = goodbye;
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
        logger.debug( "Client terminate" );
        return;
      } // switch

    } // while
  }

  void client::delete_secret_file( ) {
    if( !shared_secret_file_name_.empty() ) { 
      fs::path secret_file_path( shared_secret_file_name_ ) ;
      
      if( fs::exists( secret_file_path ) ) {
	logger.debug( (format("Removing secret file %1%") % shared_secret_file_name_ ).str() );
	fs::remove( secret_file_path );
      }
    }
  }

}
