#include "copy_command.hpp"
#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>
#include <assert.h>

namespace ucp {

  copy_command::copy_command( const po::variables_map& vm )
    :direction(none), port( default_port ) {

    vector< string > args = vm["copy-command"].as< vector< string > >();
    if( args.size() != 2 ) {
      throw std::runtime_error( "Invalid arguments" );
    } 
        
    init( args[0], args[1]);
  }
  copy_command::copy_command( const string& left_command, const string& right_command  ) 
    :direction( none ), port( default_port ) {
    init( left_command, right_command );
    
  }

  copy_command::~copy_command( ) {
  }

  void copy_command::init( const string& left_command, const string& right_command ) {

    if( contains_remote_information( left_command ) ) {
      direction = from_remote;
      parse_remote( left_command );
      target = right_command;
   } else if( contains_remote_information( right_command )) {
      direction = to_remote;
      parse_remote( right_command );
      source = left_command;
    }

    if( direction == none ) {
      throw std::runtime_error( "Invalid arguments" ) ;
    }
    

  }

  bool copy_command::contains_remote_information( const string& command ) const { 
    static const boost::regex regex( "^[A-Za-z0-9_]+@[A-Za-z0-9_\\.]+:([0-9]{1,5}:)?(~?[A-Za-z0-9_\\-\\./]+)?$");
    return boost::regex_match( command, regex );
  }

  void copy_command::parse_remote( const string& remote_command ) {
    size_t amp_pos = remote_command.find( "@" );
    user = remote_command.substr( 0, amp_pos );
    size_t colon_pos = remote_command.find( ":", amp_pos + 1 );

    host = remote_command.substr( amp_pos + 1, ( colon_pos - amp_pos - 1) );

    size_t end_colon_pos = remote_command.find( ":", colon_pos + 1 );

    if( end_colon_pos != -1 ) {
      string raw = remote_command.substr( colon_pos + 1, (end_colon_pos - colon_pos - 1) );
      port = boost::lexical_cast<int>( raw );
      colon_pos = end_colon_pos;
    }

    if( direction == from_remote ) {
      source = remote_command.substr( colon_pos + 1 );
    } else {
      target = remote_command.substr( colon_pos + 1 );
    }
  } 
}
