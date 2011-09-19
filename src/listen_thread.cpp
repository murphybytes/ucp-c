#include "listen_thread.hpp"
#include "connection_handler.hpp"

namespace ucp {
  listen_thread::listen_thread( UDTSOCKET socket )
    :socket_(socket),
     runnable_(true)
 
     
  {

    if( UDT::ERROR == UDT::listen( socket_, MAX_PENDING_CONNECTIONS ) ) {
      throw std::runtime_error( (format("Listen failed: %1%") %
				 UDT::getlasterror().getErrorMessage() ).str() );
    }

  }
  
  listen_thread::~listen_thread() {
    
    UDT::close( socket_ );
  }

  void listen_thread::operator()() {
   
    try {
      logger.debug("fired up listener thread...");
      sockaddr_storage clientaddr;
      int addrlen = sizeof(clientaddr);
 
      while(runnable_) {
	UDTSOCKET receive_socket = UDT::accept(socket_, (sockaddr*)&clientaddr, &addrlen);
	if( UDT::INVALID_SOCK == receive_socket ) {
	  throw std::runtime_error( (format("Could not create listener socket: %1%") % 
				     UDT::getlasterror().getErrorMessage() ).str() );
	}

	char clienthost[NI_MAXHOST];
	char clientservice[NI_MAXSERV];
	getnameinfo( (sockaddr*)&clientaddr, addrlen, clienthost, sizeof(clienthost),
		     clientservice, sizeof(clientservice), NI_NUMERICHOST|NI_NUMERICSERV);
	logger.debug( (format("New Connection: %1% : %2%") % 
		       clienthost % clientservice ).str());
	connection_handler handler( shared_ptr<UDTSOCKET>(new UDTSOCKET(receive_socket)) );
	boost::thread thread( connection_handler );
    
      }

    } catch( const std::exception& e ) {
      logger.error( e.what() );
    }

  }
}
