#ifndef __CONNECTION_HPP__
#define __CONNECTION_HPP__


#include "application.hpp"

namespace ucp {
  class connection : public application {
    
    friend shared_ptr<application> get_application( const po::variables_map& );
    
    connection( const po::variables_map& ) ;

  public :

    virtual ~connection() ;
    virtual void run() ;
    virtual void signal( int sig ) ;
  
  };
}

#endif
