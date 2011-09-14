#ifndef __LISTENER_HPP__
#define __LISTENER_HPP__


#include "application.hpp"

namespace ucp {
  class listener : public application {

    friend shared_ptr<application> get_application( const po::variables_map& );
    listener(const po::variables_map& );
  public:
    virtual ~listener();
    virtual void run() const ;
  };
}




#endif
