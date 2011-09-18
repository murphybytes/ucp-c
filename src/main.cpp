
#include "ucp_config.h"
#include "ucp.hpp"
#include "application.hpp"



int main( int argc, char* argv[] ) {
    int result = ucp::SUCCESS;
    unsigned int log_level = 0;
    vector< string > args;
    po::options_description desc("ucp options");
    desc.add_options()
	("help,h", "Show help message.")
	("version,v", "Show version.")
	("verbose,V", "Verbose output.")
	("log-level,l", po::value< unsigned int >( &log_level )->default_value( 3 ),  "Log level, 3 most verbose, 0 least verbose" )
	("listener,L", "Run in listener mode.")
	("copy-command", po::value< vector< string > >(&args), "Client copy command source-file user@host:port:dest-file" )
	;
    po::positional_options_description pod;
    pod.add( "copy-command", -1 );
    po::variables_map vm;
    po::store(po::command_line_parser( argc, argv ).options(desc).positional(pod).run(), vm );
    po::notify(vm);

    ucp::logger.level() = log_level;
    ucp::logger.debug( "ucp starting" );

    if( vm.count("help") ) {
	cout << desc << endl;
	return result;
    }

    if( vm.count("version") ) {
	cout << "ucp - Version: " << ucp_VERSION_MAJOR << "." << ucp_VERSION_MINOR << endl;
    }
    

    try {
	ucp::application_ptr application_ptr = ucp::get_application( vm );
	application_ptr->run();
    } catch( const exception& e ) {
	ucp::logger.error(  e.what() );
	result = ucp::ERROR;
    }      

    ucp::logger.debug( "ucp finished" );
    return result;
}
