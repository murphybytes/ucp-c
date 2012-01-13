#include "client.hpp"
#include "listener.hpp"
#include "connection.hpp"

namespace ucp {



  shared_ptr<application> get_application( const po::variables_map& command_arguments ) {
    logger.debug("Called get_application" );
    shared_ptr<application> result ;
    
    if( command_arguments.count("listener") ) {
      result = shared_ptr<application>( new listener( command_arguments ) );
    } else if( command_arguments.count( "connection-handler" ) ) {
	result = shared_ptr<application>( new connection( command_arguments ) );
      } else {
      result = shared_ptr<application>( new client( command_arguments ) );
    }

    return result;
  }

  int_t get_file_size( const string& file_name ) {
    fs::path path( file_name);

    if( !fs::exists( path ) ) {
      throw std::runtime_error( (format("File '%1%' does not exist") % file_name ).str() ); 
    }

    if( !fs::is_regular( path ) ) {
      throw std::runtime_error( (format("File '%1%' is not a regular file") % file_name).str() );
    }
    
    return fs::file_size( file_name );
  }



}
