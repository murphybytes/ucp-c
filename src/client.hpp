#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

#include "application.hpp"

namespace ucp {
  class client : public application {

    friend shared_ptr<application> get_application( const po::variables_map& );
    client(const po::variables_map& );
  public:
    virtual ~client();
    virtual void run() const; 
  };
}

#endif
