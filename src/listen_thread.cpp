#include "listen_thread.hpp"
#include "connection_handler.hpp"

namespace ucp {
  listen_thread::listen_thread(unsigned int port )
    : socket_(0), runnable_(true), port_(port)
  {

 
 
  }
  
  listen_thread::~listen_thread() {
    if(socket_) {
      UDT::close( socket_ );
    }
   
  }

  void listen_thread::operator()() {
   
    try {
      logger.debug("fired up listener thread...");

      addrinfo hints;
      addrinfo* res;
      memset( &hints, 0, sizeof(addrinfo));
      
      hints.ai_flags = AI_PASSIVE;
      hints.ai_family = AF_INET;
      hints.ai_socktype = SOCK_STREAM;

      logger.debug( (format("Preparing to listen on port %1%") % port_ ).str() );
     
      string service = boost::lexical_cast<string>( port_ );		    

      if( 0 != getaddrinfo(NULL, service.c_str(), &hints, &res)) {
	throw std::runtime_error( (format("Port %1% is illegal or busy") % service.c_str() ).str() );
      }

      socket_  = UDT::socket(res->ai_family, res->ai_socktype, res->ai_protocol);

      if( UDT::ERROR == UDT::bind( socket_, res->ai_addr, res->ai_addrlen )) {
	throw std::runtime_error( (
				   format("Bind failed: %1%") %
				   UDT::getlasterror().getErrorMessage() 
				   ).str() );
      }
      
      freeaddrinfo( res );

      logger.info( (format("Server is ready at port: %1%") % port_ ).str() );

      sockaddr_storage clientaddr;
      int addrlen = sizeof(clientaddr);

      if( UDT::ERROR == UDT::listen( socket_, MAX_PENDING_CONNECTIONS ) ) {
	throw std::runtime_error( (format("Listen failed: %1%") %
				   UDT::getlasterror().getErrorMessage() ).str() );
      }

      while(runnable_) {
	logger.debug("waiting for connection....");
	UDTSOCKET receive_socket = UDT::accept(socket_, (sockaddr*)&clientaddr, &addrlen);
	if( UDT::INVALID_SOCK == receive_socket ) {
	  throw std::runtime_error( (format("Could not create listener socket: %1%") % 
				     UDT::getlasterror().getErrorMessage() ).str() );
	}

	logger.debug( "Got connection...");
	char clienthost[NI_MAXHOST];
	char clientservice[NI_MAXSERV];
	getnameinfo( (sockaddr*)&clientaddr, addrlen, clienthost, sizeof(clienthost),
		     clientservice, sizeof(clientservice), NI_NUMERICHOST|NI_NUMERICSERV);
	logger.debug( (format("New Connection: %1% : %2%") % 
		       clienthost % clientservice ).str());
	connection_handler handler( receive_socket);
	boost::thread thread( handler );
    
      }


    } catch( const std::exception& e ) {
      logger.error( e.what() );
    }

  }
}
