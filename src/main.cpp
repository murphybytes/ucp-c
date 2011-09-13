#include <iostream>
#include <boost/program_options.hpp>
#include "ucp_config.h"

using std::cout;
using std::endl;

namespace po = boost::program_options;

int main( int argc, char* argv[] ) {
    po::options_description desc("ucp options");
    desc.add_options()
	("help", "Show help message.")
	("version,v", "Show version.")
	;
    po::variables_map vm;
    po::store(po::parse_command_line( argc, argv, desc), vm );
    po::notify(vm);
    if( vm.count("version") ) {
	cout << "ucp - Version: " << ucp_VERSION_MAJOR << "." << ucp_VERSION_MINOR << endl;
    }

    return 0;
}
