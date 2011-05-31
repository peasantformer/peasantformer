#ifndef PEASANTFORMER_Data_ConnectionPending
#define PEASANTFORMER_Data_ConnectionPending

/// @file

#include <Data/Connection/Connection.h>

/// Pending connection class
class ConnectionPending : public Connection {
	public:
	
		/// Dummy constructor
		ConnectionPending();
		
		/// Real constructor.
		///
		/// \param [in] sockfd is a socket fd
		/// \param [in] remote_addr is remote addr struct
		/// \param [in] buffsize is buffers size
		ConnectionPending(int sockfd, struct sockaddr_storage remote_addr, size_t buffsize);
		
		/// Destructor
		virtual ~ConnectionPending() {}
};

#endif
