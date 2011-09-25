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

    UDT::startup();
  }

  listener::~listener() {
    logger.debug( "destroy listener" );
    UDT::cleanup();
  }

  void listener::run() {

    logger.debug( "run listener" );
    
    try {
      
      listen_thread listener( port_ );
      thread t( listener );
      
      condition_.wait( lock_ );
      logger.info( "Application recieved shut down signal...");
      listener.shutdown();
    } catch( const std::exception& e ) {
      logger.error( e.what() );
    }

  }

  void listener::signal( int sig ) {
    logger.debug( "got signal" );
    condition_.notify_one();
  }

}
