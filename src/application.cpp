#include "client.hpp"
#include "listener.hpp"

namespace ucp {


  shared_ptr<application> get_application( const po::variables_map& command_arguments ) {
    
    shared_ptr<application> result ;
    
    if( command_arguments.count("listener") ) {
      result = shared_ptr<application>( new listener( command_arguments ) );
    } else {
      result = shared_ptr<application>( new client( command_arguments ) );
    }

    return result;
  }



}
