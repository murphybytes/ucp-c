#ifndef __STATES_HPP__
#define __STATES_HPP__

namespace ucp {
  namespace send {
    enum txfer_state {
      file_name,
      file_name_ack,
      file_size,
      file_size_ack,
      file_send,
      wait_for_req_file,
      wait_for_file_size_req,
      wait_for_send_ack,
      error_ack,
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
    error_ack,
    term
  };
  }
}


#endif


