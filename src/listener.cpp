#include "listener.hpp"
#include "listen_thread.hpp"

using ucp::logger;
using std::fstream;
using std::ios;

namespace ucp { 

  listener::listener( const po::variables_map& command_arguments ) 
    :run_(true),
     port_(9090),
     lock_(mutex_),
     pid_file_path_((format("%1%/ucp.pid") % command_arguments["pid-directory"].as<string>() ).str()),
     daemonize_( command_arguments.count("daemonize") )
  {
    logger.debug( "create listener" );
    if( command_arguments.count( "listen-port" ) ) {
      port_ = command_arguments["listen-port"].as<unsigned int>();
    }

    if( daemonize_ ) {
      daemonize();
    }

    UDT::startup();
  }

  listener::~listener() {
    logger.debug( "destroy listener" );
    UDT::cleanup();

    if( daemonize_ ) {
      if( fs::exists( pid_file_path_ ) ) {
	  fs::remove_all( pid_file_path_ ) ;
	  logger.debug( (format("Removed lock file %1%") % pid_file_path_ ).str() );
	}

    }

  }

  void listener::daemonize() {
    if( fs::exists( pid_file_path_ ) ) {
      throw std::runtime_error( (format("Program exiting. Lockfile %1% exists") %
				 pid_file_path_).str() );
    }

    
    fstream pid_file( pid_file_path_.c_str(), ios::out | ios::trunc );
    pid_file << getpid();
    pid_file.close();
    
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
