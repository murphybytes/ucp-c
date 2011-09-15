#include "listener.hpp"

using ucp::logger;

namespace ucp { 

  listener::listener( const po::variables_map& command_arguments ) {
    logger.debug( "create listener" );
  }

  listener::~listener() {
    logger.debug( "destroy listener" );
  }

  void listener::run() const {
    logger.debug( "run listener" );
  }

}
