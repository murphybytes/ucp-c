#ifndef __STATES_HPP__
#define __STATES_HPP__

namespace ucp {
  namespace send {
  enum txfer_state {
    wait_for_req_file,
    wait_for_file_size_req,
    wait_for_send_ack,
    error,
    term
  };
  }

  enum recv_file_state {
  };
}


#endif


