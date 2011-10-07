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

  namespace recv {
  enum txfer_state {
    wait_for_file_size,
    ack_file_size,
    receive_file,
    send_file_name,
    send_file_ack,
    receive_file_size,
    request_file_size,
    error,
    term
  };
  }
}


#endif


