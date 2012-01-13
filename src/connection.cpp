#include "connection.hpp"

using ucp::logger;

namespace ucp {

  connection::connection( const po::variables_map& commands ) {
    logger.debug( "enter connection" );
  }

  connection::~connection() {
  }

  void connection::run() {
  }

  void connection::signal( int sig ) {
  }

}
