#ifndef PEASANTFORMER_Data_ConnectionAccepted
#define PEASANTFORMER_Data_ConnectionAccepted

/// @file

#include <Data/Connection/ConnectionPending.h>

/// Pending connection class
class ConnectionAccepted : public ConnectionPending {
	public:
	
		/// Dummy constructor
		ConnectionAccepted();
		
		/// Real constructor.
		///
		/// \param [in] sockfd is a socket fd
		/// \param [in] remote_addr is remote addr struct
		/// \param [in] buffsize is buffers size
		ConnectionAccepted(int sockfd, struct sockaddr_storage remote_addr, size_t buffsize);
		
		/// Pending-copy constructor.
		///
		/// \param [in] pend is pending constructor to promote
		ConnectionAccepted(const ConnectionPending &pend);
		
		/// Destructor
		virtual ~ConnectionAccepted() {}
};

#endif
