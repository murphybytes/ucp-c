#ifndef __LISTEN_THREAD_HPP__
#define __LISTEN_THREAD_HPP__


#include "application.hpp"

namespace ucp {

  class listen_thread {
    bool runnable_;
    UDTSOCKET socket_;
    unsigned int port_;
    po::variables_map command_arguments_;
  public:
    listen_thread(const po::variables_map& command_arguments  );
    virtual ~listen_thread() ;
    void shutdown() { runnable_ = false; }
    void operator()(shared_ptr<std::fstream>);
  };

}

#endif
