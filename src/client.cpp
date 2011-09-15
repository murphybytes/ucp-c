#include "client.hpp"

using ucp::logger;

namespace ucp { 

  client::client( const po::variables_map& command_arguments ) {
    logger.debug(  "create client" );
  }

  client::~client() {
    logger.debug( "destroy client" );
  }

  void client::run() const {
    logger.debug( "run client" );
  }

}
