#ifndef __ENCRYPTION_SERVICE_HPP__
#define __ENCRYPTION_SERVICE_HPP__

#include "application.hpp"

namespace ucp {

  class encryption_service {
  public:
    void generate_shared_secret( byte_string& shared_secret ) const;
    virtual ~encryption_service() {
    }
  };

}


#endif
