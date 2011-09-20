#include "messaging.hpp"

namespace ucp {

  messaging::~messaging() {

  }
  
  void messaging::close() {
    UDT::close( *socket_ );
  }

  void messaging::send( const string& msg ) {
    if( !msg.empty() ) {
      int result = UDT::send( *socket_, msg.c_str(), msg.size() + 1, 0 );
      if( UDT::ERROR == result ) {
	throw std::runtime_error( UDT::getlasterror().getErrorMessage() );
      }
    }
  }

  void messaging::send( int_t msg ) {
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
  }


}
