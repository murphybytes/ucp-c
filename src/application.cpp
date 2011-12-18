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
