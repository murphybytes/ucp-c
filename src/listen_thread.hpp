#ifndef __LISTEN_THREAD_HPP__
#define __LISTEN_THREAD_HPP__


#include "application.hpp"

namespace ucp {

  class listen_thread {
    bool runnable_;
    UDTSOCKET socket_;
    unsigned int port_;
  public:
    listen_thread(unsigned int port  );
    virtual ~listen_thread() ;
    void shutdown() { runnable_ = false; }
    void operator()();
  };

}

#endif
