#ifndef __UCP_HPP__
#define __UCP_HPP__

namespace ucp {
  const int SUCCESS = 0;
  const int ERROR = 1;
  const int UNUSED = 0;
  // platform independent integer for wire 
  // communication ( at least little endian platforms )
  typedef int64_t int_t;

  typedef enum {
    none,
    from_remote,
    to_remote } direction_t;

  
}


#endif
