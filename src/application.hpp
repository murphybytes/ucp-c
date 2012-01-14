#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include <udt.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <assert.h>
#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/format.hpp>
#include <boost/signal.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <vector>
#include <exception>
#include <string>
#include "ucp.hpp"
#include "log.hpp"


using std::cout;
using std::endl;
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::exception;
using boost::format;
using boost::lexical_cast;
using boost::thread;
using boost::condition_variable;
using boost::mutex;
using boost::unique_lock;

typedef std::basic_string< unsigned char > byte_string;

namespace po = boost::program_options;
namespace fs = boost::filesystem;

using boost::shared_ptr;

namespace ucp {

  const static unsigned int MAX_PENDING_CONNECTIONS = 10;

  struct application {
    virtual void run()  = 0;
    virtual void signal( int ) = 0;
  };

  typedef shared_ptr< application > application_ptr;

  class signal_handler {
    application_ptr app_;
  public:
    signal_handler( shared_ptr< application > app )
      :app_(app){}
    void operator()() {
      app_->signal(0);
    }
  };

  shared_ptr<application> get_application( const po::variables_map& command_arguments );


  int_t get_file_size( const string& file_name );
}


#endif
