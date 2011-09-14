#include "listener.hpp"


namespace ucp { 

  listener::listener( const po::variables_map& command_arguments ) {
    cout << "create listener" << endl;
  }

  listener::~listener() {
    cout << "destroy listener" << endl;
  }

  void listener::run() const {
    cout << "run listner" << endl;
  }

}
