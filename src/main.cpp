
#include "ucp_config.h"
#include "application.hpp"
#include <signal.h>

boost::signal<void()> signal_dispatcher;

void on_signal( int sig ) {
    signal_dispatcher();
} 

int main( int argc, char* argv[] ) {
    int result = ucp::SUCCESS;
    unsigned int log_level = 0;
    vector< string > args;
    po::options_description visible_options("ucp options");
    visible_options.add_options()
	("help,h", "Show help message.")
	("version,v", "Show version.")
	("verbose,V", "Verbose output.")
	("log-level,l", po::value< unsigned int >( &log_level )->default_value( 3 ),  "Log level, 3 most verbose, 0 least verbose" )
	("listener,L", "Run in listener mode.")
      ("daemonize,d", "Run as daemon. (Listener only)" )
	("listen-port,P", po::value< unsigned int >()->default_value(9090), "Port to listen on in listener mode")
        ("pid-directory,p", po::value< string >()->default_value("/var/run"), "Pid file directory if in daemon mode" )
      ("max-bandwidth", po::value< int64_t >()->default_value( -1 ), "Maximum bandwidth that a single connection can use in bytes / sec.  Default -1 (infinite)" )
	("copy-command", po::value< vector< string > >(&args), "Client copy command source-file user@host:port:dest-file" )
      
	;
    po::positional_options_description pod;
    pod.add( "copy-command", -1 );

    
    po::options_description hidden_options( "connection handler options" );
    hidden_options.add_options() 
      ( "connection-handler", "flag that this is a child process" )
      ( "connection-socket", po::value< int >(), "socket to handle connection"  )
      ;

    po::options_description all_options("all options");
    all_options.add( visible_options ).add( hidden_options );


    
    po::variables_map command_arguments;
    
    po::store(po::command_line_parser( argc, argv ).options(all_options).positional(pod).run(), command_arguments);
    po::notify(command_arguments);

    ucp::logger.level() = log_level;
    ucp::logger.debug( "ucp starting" );
    signal(SIGINT, on_signal);

    if( command_arguments.count("help") || 1 == argc  ) {
	cout << visible_options << endl;
	return result;
    }

    if( command_arguments.count("version") ) {
	cout << "ucp - Version: " << ucp_VERSION_MAJOR << "." << ucp_VERSION_MINOR << endl;
    }
    

    try {	
	ucp::application_ptr application_ptr = ucp::get_application( command_arguments );
	
	signal_dispatcher.connect( ucp::signal_handler(application_ptr) );
	application_ptr->run();
    } catch( const exception& e ) {
	ucp::logger.error(  e.what() );
	result = ucp::ERROR;
    }      

    ucp::logger.debug( "ucp finished" );
    return result;
}
