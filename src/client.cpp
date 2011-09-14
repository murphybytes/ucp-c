#include "client.hpp"

namespace ucp { 

  client::client( const po::variables_map& command_arguments ) {
    cout << "create client" << endl;
  }

  client::~client() {
    cout << "destroy client" << endl;
  }

  void client::run() const {
    cout << "run client" << endl;
  }

}
