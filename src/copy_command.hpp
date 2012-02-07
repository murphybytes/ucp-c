#ifndef __COPY_COMMAND_HPP__
#define __COPY_COMMAND_HPP__


#include "application.hpp"
#include <stdexcept>
using std::runtime_error;
using std::size_t;


namespace ucp {


  const static int default_port = 9090;
  /**
   * \brief Contains file copy instructions parsed from command line
   */
  class copy_command {
    direction_t direction;
    string user;
    string host;
    int port;
    string source;
    string target;
    string service;
  public:
    copy_command( const po::variables_map& vm );
    copy_command( const string& left_command, const string& right_command ); 
    virtual ~copy_command() ;
    direction_t get_direction() const { return direction; } 
    bool contains_remote_information( const string& command ) const ;
    const string& get_source() const { return source; }
    const string& get_target() const { return target; }
    const string& get_user() const { return user; }
    const string& get_host() const { return host; }
    const int get_port() const { return port; }
    const string& get_service() const { return service; } 
  private:
    void parse_remote( const string& remote_command ) ;
    void init( const string& left_command, const string& right_command );
  };

}

#endif
