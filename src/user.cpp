#include "user.hpp"

namespace ucp {

  void chown( const user& owner, const bfs::path& path ) {
    int result = ::chown( path.string().c_str(), owner.uid_, owner.gid_ );
  }

  user::user( const string& user_name )  {
    struct passwd* pwd = getpwnam( user_name.c_str() );
    if( NULL == pwd ) { 
      throw std::runtime_error( (format( "Attempt to retrieve system info for user %1% failed" ) % user_name ).str() ) ;
    }

    home_dir_ = bfs::path( pwd->pw_dir );
    uid_ = pwd->pw_uid;
    gid_ = pwd->pw_gid;
  }

  user::user() {
    struct passwd* pwd = getpwuid( getuid() );
    if( NULL == pwd ) { 
      throw std::runtime_error( (format( "Attempt to retrieve system info for user %1% failed" ) % getuid() ).str() ) ;
    }

    home_dir_ = bfs::path( pwd->pw_dir );
    uid_ = pwd->pw_uid;
    gid_ = pwd->pw_gid;

  }
}
