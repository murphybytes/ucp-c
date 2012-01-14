#ifndef __CONNECTION_HPP__
#define __CONNECTION_HPP__


#include "application.hpp"
#include "connection_handler.hpp"

namespace ucp {
  class connection : public application {
    
    friend shared_ptr<application> get_application( const po::variables_map& );

    connection_handler handler_;
    
    connection( const po::variables_map& ) ;

  public :

    virtual ~connection() ;
    virtual void run() ;
    virtual void signal( int sig ) ;
  
  };
}

#endif
