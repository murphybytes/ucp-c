#include <iostream>
#include <vector>
#include <string>
#include <boost/program_options.hpp>
#include "ucp_config.h"
#include "ucp.hpp"

using std::cout;
using std::endl;
using std::vector;
using std::string;

namespace po = boost::program_options;

int main( int argc, char* argv[] ) {
    int result = ucp::SUCCESS;

    po::options_description desc("ucp options");
    desc.add_options()
	("help,h", "Show help message.")
	("version,v", "Show version.")
	("verbose,V", "Verbose output.")
	("listener,L", "Run in listener mode.")
	("copy-command", po::value< vector< string > >(), "Client copy command" )
	;
    po::positional_options_description pod;
    pod.add( "copy-command", -1 );
    po::variables_map vm;
    po::store(po::command_line_parser( argc, argv ).options(desc).positional(pod).run(), vm );
    po::notify(vm);

    if( vm.count("help") ) {
	cout << desc << endl;
	return result;
    }

    if( vm.count("version") ) {
	cout << "ucp - Version: " << ucp_VERSION_MAJOR << "." << ucp_VERSION_MINOR << endl;
    }

    return result;
}
