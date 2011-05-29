#ifndef PEASANTFORMER_Data_ConnectionPending
#define PEASANTFORMER_Data_ConnectionPending

/// @file

#include <Data/Connection/Connection.h>

/// Pending connection class.
class ConnectionPending : public Connection {
	public:
		/// Dummy constructor
		ConnectionPending();
		/// Real constructor
		ConnectionPending(
			int sockfd,                          ///< [in] socket file descriptor
			struct sockaddr_storage remote_addr, ///< [in] remote addr struct
			size_t buffsize                      ///< [in] buffer size
		);
		// Destructor
		virtual ~ConnectionPending() {}
};


#endif
