#ifndef __USER_HPP__
#define __USER_HPP__

#include "application.hpp"

namespace bfs = boost::filesystem3;


namespace ucp {
  /**
   * \brief Pertains to operating system user
   *
   *  Class contains data and behaviour associated with management of 
   *  a Linux OS user.  
   */
  class user {
    bfs::path home_dir_;
    int uid_;
    int gid_;

    friend void chown( const user&, const bfs::path& );

  public:
    user() ;
    user( const string& user_name ) ;
    ~user() {}
    const bfs::path& get_home_directory() const { return home_dir_; }
    
    
  };
  

  void chown( const user& owner, const bfs::path& path ) ; 

}


#endif
