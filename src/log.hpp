#ifndef __LOG_HPP__
#define __LOG_HPP__


#include <iostream>
using std::cout;
using std::endl;

namespace ucp {
   
  class log {
    unsigned int log_level ;

    void log_message( unsigned int level, const char* msg ) const {
      switch( level ) {
      case DEBUG:
	cout << "DEBUG " << msg << endl;
	break;
      case INFO:
	cout << "INFO  " << msg << endl;
	break;
      case WARN:
	cout << "WARN  " << msg << endl;
	break;
      case ERROR:
	cout << "ERROR " << msg << endl;
	break;
      }
    }
  public:
    static const unsigned int DEBUG = 3;
    static const unsigned int INFO = 2;
    static  const unsigned int WARN = 1;
    static const unsigned int ERROR = 0;
    
    log() :log_level(DEBUG) {}
    virtual ~log() {}
    
    unsigned int& level() { return log_level; }
    
    void debug( const char* msg ) const {
      if( log_level >= DEBUG ) {
	log_message( DEBUG, msg );
      } 
    }

    void error( const char* msg ) const {
      if( log_level >= ERROR ) {
	log_message( ERROR, msg );
      } 
    }

    void info( const char* msg ) const {
      if( log_level >= INFO ) {
	log_message( INFO, msg );
      } 
    }
    
    void warn( const char* msg ) const {
      if( log_level >= WARN ) {
	log_message( WARN, msg ); 
      }
    }

  };
  
  static log logger;

}


#endif
