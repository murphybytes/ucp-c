#include "connection_handler.hpp"

namespace ucp {

  
  void connection_handler::operator()() {
    logger.debug("started connection handler");
    // make a copy of the shared pointer, this thread will live
    // longer than inclosing class
    shared_ptr<UDTSOCKET> socket_ptr = socket_ptr_;
    
    // get user    

    
      
  } 


}
