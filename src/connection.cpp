#include "connection.hpp"

using ucp::logger;

namespace ucp {

  connection::connection( const po::variables_map& commands )
    :handler_(commands)     {
    logger.debug( "enter connection" );

  }

  connection::~connection() {
  }

  void connection::run() {
    handler_(logger.get_fstream());
  }

  void connection::signal( int sig ) {
  }

}
