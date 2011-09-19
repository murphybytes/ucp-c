#ifndef __LISTENER_HPP__
#define __LISTENER_HPP__


#include "application.hpp"

namespace ucp {
  class listener : public application {

    friend shared_ptr<application> get_application( const po::variables_map& );
    
    bool run_;
    unsigned int port_;
    mutex mutex_;
    unique_lock< mutex > lock_;				
    condition_variable condition_;

    listener(const po::variables_map& );
    const char* get_service() const { return lexical_cast< string >( port_ ).c_str() ; }
  public:
    virtual ~listener();
    virtual void run()  ;
    virtual void signal( int sig );
  };
}




#endif
