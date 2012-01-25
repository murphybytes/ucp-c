#include "user.hpp"

namespace ucp {

  void chown( const user& owner, const bfs::path& path ) {
    int result = ::chown( path.string().c_str(), owner.uid_, owner.gid_ );
  }

  user::user( const string& user_name ) 
    :user_name_(user_name) {
    struct passwd* pwd = getpwnam( user_name_.c_str() );
    if( NULL == pwd ) { 
      throw std::runtime_error( (format( "Attempt to retrieve system info for user %1% failed" ) % user_name ).str() ) ;
    }

    home_dir_ = bfs::path( pwd->pw_dir );
    uid_ = pwd->pw_uid;
    gid_ = pwd->pw_gid;
  }
}
