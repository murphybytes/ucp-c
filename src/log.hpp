#ifndef __LOG_HPP__
#define __LOG_HPP__


#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <boost/shared_ptr.hpp>

using std::cout;
using std::endl;
using std::string;
using boost::shared_ptr;

namespace ucp {
  /**
   *  \brief Provides logging service for application.  
   */
  class log {
    unsigned int log_level ;
    shared_ptr<std::fstream>  out_;


  void log_message( unsigned int level, const char* msg )  {
    std::ostream* pout = out_ != NULL ? out_.get() : &cout;

      switch( level ) {
      case DEBUG:
	(*pout) << "DEBUG " << msg << endl;
	break;
      case INFO:
	(*pout) << "INFO  " << msg << endl;
	break;
      case WARN:
	(*pout) << "WARN  " << msg << endl;
	break;
      case ERROR:
	(*pout) << "ERROR " << msg << endl;
	break;
      }

    }
  public:
    static const unsigned int DEBUG = 3;
    static const unsigned int INFO = 2;
    static  const unsigned int WARN = 1;
    static const unsigned int ERROR = 0;
    
    log() :log_level(DEBUG) {}
    virtual ~log() { 
      if( out_ != NULL ) {
	out_->close();
      }
    }
    /**
     *  This function is used to pass heap allocated file pointer from thread to thread
     *  in daemon mode.  Logger doesn't work otherwise since we use a static global
     *  instance to log and statics are stack based storage and threads have their own
     *  stack.
     */
    shared_ptr<std::fstream> get_fstream() { return out_; }
    /**
     *  See get_fstream
     */ 
    void set_fstream( shared_ptr<std::fstream> fs ) { out_ = fs; }

    void daemon_log_to_file( const string& log ) {
      out_ = shared_ptr<std::fstream>(new std::fstream( log.c_str(),  std::ios::app | std::ios::out ) );
      chmod( log.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH );

    }
    
    unsigned int& level() { return log_level; }
    
    void debug( const char* msg )  {
      if( log_level >= DEBUG ) {
	log_message( DEBUG, msg );
      } 
    }
    
    void debug( const std::string& msg )  {
      debug( msg.c_str() );
    }

    void error( const char* msg )  {
      if( log_level >= ERROR ) {
	log_message( ERROR, msg );
      } 
    }

    void error( const std::string& msg )  {
      error( msg.c_str() );
    }

    void info( const char* msg ) {
      if( log_level >= INFO ) {
	log_message( INFO, msg );
      } 
    }

    void info( const std::string& msg ) {
      info( msg.c_str() );
    }
    
    void warn( const char* msg )  {
      if( log_level >= WARN ) {
	log_message( WARN, msg ); 
      }
    }

    void warn( const std::string& msg )  {
      warn( msg.c_str() );
    }

  };

  log& logger(); 

  

}


#endif
