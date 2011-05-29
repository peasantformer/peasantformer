#ifndef PEASANTFORMER_Data_ConnectionAccepted
#define PEASANTFORMER_Data_ConnectionAccepted

/// @file

#include <Data/Connection/Connection.h>
#include <Data/Connection/ConnectionPending.h>

/// Accepted connection class.
class ConnectionAccepted : public ConnectionPending {
	public:
		/// Dummy constructor
		ConnectionAccepted();
		/// Real constructor
		ConnectionAccepted(
			int sockfd,                          ///< [in] socket file descriptor
			struct sockaddr_storage remote_addr, ///< [in] remote addr struct
			size_t buffsize                      ///< [in] buffer size
		);
		/// Pending -> Accepted constructor
		ConnectionAccepted(ConnectionPending);
};


#endif
