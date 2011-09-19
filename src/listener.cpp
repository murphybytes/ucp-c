#include "listener.hpp"
#include "listen_thread.hpp"

using ucp::logger;

namespace ucp { 

  listener::listener( const po::variables_map& command_arguments ) 
    :run_(true),
     port_(9090),
     lock_(mutex_)
  {
    logger.debug( "create listener" );
    if( command_arguments.count( "listen-port" ) ) {
      port_ = command_arguments["listen-port"].as<unsigned int>();
    }
  }

  listener::~listener() {
    logger.debug( "destroy listener" );
  }

  void listener::run() {
    UDT::startup();
    logger.debug( "run listener" );
    
    try {
      addrinfo hints;
      addrinfo* res;
      memset( &hints, 0, sizeof(addrinfo));
      
      hints.ai_flags = AI_PASSIVE;
      hints.ai_family = AF_INET;
      hints.ai_socktype = SOCK_STREAM;
      
      if( 0 != getaddrinfo(NULL, get_service(), &hints, &res)) {
	throw std::runtime_error( (format("Port %1% is illegal or busy") % get_service() ).str() );
      }

      UDTSOCKET listener_socket  = UDT::socket(res->ai_family, res->ai_socktype, res->ai_protocol);      
      
      if( UDT::ERROR == UDT::bind( listener_socket, res->ai_addr, res->ai_addrlen )) {
	throw std::runtime_error( (
				   format("Bind failed: %1%") %
				   UDT::getlasterror().getErrorMessage() 
				   ).str() );
      }
      
      freeaddrinfo( res );

      logger.info( (format("Server is ready at port: %1%") % port_ ).str() );
      
      listen_thread listener( listener_socket );
      thread t( listener );
      condition_.wait( lock_ );
      logger.info( "Application recieved shut down signal...");
      listener.shutdown();
    } catch( const std::exception& e ) {
      logger.error( e.what() );
    }

    UDT::cleanup();
  }

  void listener::signal( int sig ) {
    logger.debug( "got signal" );
    condition_.notify_one();
  }

}
