#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include <udt.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/format.hpp>
#include <iostream>
#include <vector>
#include <exception>
#include <string>
#include "log.hpp"

using std::cout;
using std::endl;
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::exception;
using boost::format;

namespace po = boost::program_options;
using boost::shared_ptr;

namespace ucp {
  struct application {
    virtual void run() const = 0;
  };

  typedef shared_ptr< application > application_ptr;

  shared_ptr<application> get_application( const po::variables_map& command_arguments );

}


#endif
