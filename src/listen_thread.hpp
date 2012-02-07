#ifndef __LISTEN_THREAD_HPP__
#define __LISTEN_THREAD_HPP__


#include "application.hpp"

namespace ucp {
  /** 
   *  \brief Handles server listener socket
   *
   *  Class handles server side socket that listens for incoming connections.  See connection_handler.hpp 
   *  for code that handles conversation with external clients. 
   */
  class listen_thread {
    bool runnable_;
    UDTSOCKET socket_;
    unsigned int port_;
    po::variables_map command_arguments_;
  public:
    listen_thread(const po::variables_map& command_arguments  );
    virtual ~listen_thread() ;
    /**
     * Causes application to stop listening for incoming connections
     */ 
    void shutdown() { runnable_ = false; }
    /**
     * Thread handler function
     *
     * \param stm output stream for logging
     */
    void operator()(shared_ptr<std::fstream> stm);
  };

}

#endif
