#ifndef __LISTEN_THREAD_HPP__
#define __LISTEN_THREAD_HPP__


#include "application.hpp"

namespace ucp {

  class listen_thread {
    UDTSOCKET socket_;
    bool runnable_;
  public:
    listen_thread( UDTSOCKET socket );
    virtual ~listen_thread() ;
    void shutdown() { runnable_ = false; }
    void operator()();
  };

}

#endif
