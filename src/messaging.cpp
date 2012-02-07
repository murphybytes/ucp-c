#include "messaging.hpp"
#include "ucp_fstream.hpp"
#include <sys/types.h>
#include <pwd.h>


using std::ios;

namespace ucp {
  void messaging::enable_encryption( const string& secret_file_name ) {
    if( user_ == NULL ) {
      encryptor_ = shared_ptr< encryption_service >( new encryption_service( secret_file_name ) );  
    } else {
      encryptor_ = shared_ptr< encryption_service >( new encryption_service( user_->get_home_directory().string(), secret_file_name ) );
    }
    
  }


  void messaging::set_user( const string& user_name ) {
    logger().debug( (format("setting user %1% for session") % user_name ).str() );
    user_ = shared_ptr<user>( new user( user_name ) );
     
  }

  void messaging::send_file( const string& file_name ) {
    logger().debug( (format("Preparing to send file %1%") % file_name ).str());
    ucp::fstream in_stream( file_name, ios::in | ios::binary, encryptor_ );
    int_t file_size = get_file_size( file_name );
    int_t offset = 0;
    UDT::TRACEINFO trace;
    UDT::perfmon( *socket_, &trace );
    
    if( UDT::ERROR == UDT::sendfile( *socket_, in_stream, offset, file_size )) {
      throw std::runtime_error( UDT::getlasterror().getErrorMessage() );
    }

    UDT::perfmon( *socket_, &trace );
    logger().info( (format("Transfer speed %1% Mbits/sec") % trace.mbpsSendRate ).str() );
    
  }
  
  void messaging::receive_file_from_remote( const string& file_name, int_t file_size ) {
    assert( role_ == server_role );

    bfs::path path( user_->get_home_directory() );
    path /=  file_name ;

    receive_file( path, file_size );
    
    ucp::chown( *user_, path );
  }

  void messaging::receive_file( const bfs::path& path, int_t file_size ) {
    receive_file( path.string(), file_size );
  }

  void messaging::receive_file( const string& file_name, int_t file_size ) {
    logger().debug( (format("Preparing to receive file %1% of size %2%.") % file_name % file_size ).str() ); 
    ucp::fstream out_stream( file_name, ios::binary | ios::trunc | ios::out, encryptor_ );
    int_t received_size = 0;
    int_t offset = 0 ;
    if( UDT::ERROR == ( received_size = UDT::recvfile( *socket_, out_stream, offset, file_size ))) {
      throw std::runtime_error( UDT::getlasterror().getErrorMessage() );
    }  
    logger().debug( (format("%1% was received successfully") % file_name ).str() );
  }

  void messaging::trace_send( const string& msg ) {
    string role_name = (client_role == role_ ? "CLIENT" : "SERVER");
    logger().debug((format("%1% SEND: %2%") % role_name % msg ).str());
  }

  void messaging::trace_recv( const string& msg ) {
    string role_name = (client_role == role_ ? "CLIENT" : "SERVER");
    logger().debug( (format( "%1% RECV: %2%" ) % role_name % msg ).str() );
  }

  void messaging::trace_send( int_t msg ) {
    string str_msg = boost::lexical_cast< string >( msg );
    trace_send( str_msg );
  }

  void messaging::trace_recv( int_t msg ) {
    string str_msg = boost::lexical_cast< string >( msg );
    trace_recv( str_msg );
  }

  messaging::~messaging() {

  }
  
  void messaging::close() {
    logger().debug((format("Closing socket %1%") % *socket_).str());
    UDT::close( *socket_ );
  }

  void messaging::send( const string& msg ) {
    trace_send( msg );
    if( !msg.empty() ) {
      string encrypted;

      if( encryptor_ != NULL ) {
	encryptor_->encrypt( msg, encrypted );
      } else {
	encrypted = msg;
      }

      int result = UDT::send( *socket_, encrypted.data(), encrypted.size(), 0 );
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
    
    int received = UDT::recv( *socket_, (char*)msg, sizeof(int_t), 0 );
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
    string encrypted( protocol_buffer, received );
    
    if( encryptor_ != NULL ) {
      encryptor_->decrypt( encrypted, buffer );
    } else {
      buffer = encrypted ;
    }

    trace_recv( buffer );
  }


  

}
