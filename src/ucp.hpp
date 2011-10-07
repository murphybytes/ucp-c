#ifndef __UCP_HPP__
#define __UCP_HPP__

namespace ucp {
  const int SUCCESS = 0;
  const int ERROR = 1;
  // platform independent integer for wire 
  // communication ( at least little endian platforms )
  typedef int64_t int_t;
}


#endif
