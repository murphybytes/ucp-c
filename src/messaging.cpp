#include "messaging.hpp"

namespace ucp {
  
  void messaging::trace_send( const string& msg ) {
    string role_name = (client_role == role ? "CLIENT" : "SERVER");
    logger.debug((format("%1% SEND: %2%") % msg ).str());
  }

  void messaging::trace_recv( const string& msg ) {
    string role_name = (client_role == role ? "CLIENT" : "SERVER");
    logger.debug( (format( "%1% RECV: %2%" ) % msg ).str() );
  }

  void messaging::trace_send( int_t msg ) {
    string str_msg = boost::lexical_cast< string >( msg );
    trace_send( str_msg );
  }

  void messaging::trace_recv( int_t msg ) {
    string str_msg = boost::lexical_cast< string >( msg );
    trace_recv( msg );
  }

  messaging::~messaging() {

  }
  
  void messaging::close() {
    logger.debug((format("Closing socket %1%") % *socket_).str());
    UDT::close( *socket_ );
  }

  void messaging::send( const string& msg ) {
    trace_send( msg );
    if( !msg.empty() ) {
      int result = UDT::send( *socket_, msg.c_str(), msg.size() + 1, 0 );
      if( UDT::ERROR == result ) {
	throw std::runtime_error( UDT::getlasterror().getErrorMessage() );
      }
    }
  }

  void messaging::send( int_t msg ) {
    trace_send( msg );
    int result = UDT::send( *socket_, reinterpret_cast<char*>( &msg ), sizeof(int_t), 0 );
    if( UDT::ERROR == result ) {
	throw std::runtime_error( UDT::getlasterror().getErrorMessage() );      
    }
  }

  void messaging::receive( int_t* msg ) {
    int received = UDT::recv( *socket_, reinterpret_cast<char*>( msg ), sizeof(int_t), 0 );
    if( UDT::ERROR == received ) {
      throw std::runtime_error( UDT::getlasterror().getErrorMessage() );
    }
    trace_recv( *msg );
  }

  void messaging::receive( string& buffer ) {
    buffer.clear();
    
    int received = UDT::recv( *socket_, protocol_buffer, protocol_buffer_size, 0 );
    if( UDT::ERROR == received ) {
      throw std::runtime_error( UDT::getlasterror().getErrorMessage() );
    }
    
    if( !(received < (protocol_buffer_size - 1)) ) {
      throw std::runtime_error( "Protocol message too long" );
    } 

    protocol_buffer[received] = 0; // null term
    buffer = protocol_buffer;
    trace_recv( buffer );
  }


}
