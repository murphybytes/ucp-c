#include "application.hpp"

namespace ucp {
  log& logger() {
    static log l;
    return l;
  }
}
