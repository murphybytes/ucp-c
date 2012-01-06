#ifndef __ENCRYPTION_SERVICE_HPP__
#define __ENCRYPTION_SERVICE_HPP__

#include "application.hpp"

namespace ucp {

  class encryption_service {
  public:
    void generate_shared_secret( byte_string& shared_secret ) const;
    void write_shared_secret_to_file( const byte_string& shared_secret, string& file_name ) const ;
    void read_shared_secret_from_file( const string& file_name, byte_string& shared_secret ) const;
    void get_random_filename( string& file_name ) const;
    void send_shared_secret_to_remote_host( const string& remote_user, const string& remote_host, const string& file_name ) const;

    virtual ~encryption_service() {
    }
  };

}


#endif
