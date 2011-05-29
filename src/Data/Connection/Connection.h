#ifndef PEASANTFORMER_Data_Connection
#define PEASANTFORMER_Data_Connection

/// @file

#include <Data/CircularBuffer/CircularBuffer.h>
#include <Proto/Network/NetworkHighLevel.h>


/// Basic connection class.
class Connection {
	public:
		int sockfd;                             ///< socket file descriptor
		size_t buffsize;                        ///< circular and plain buffers size
		CircularBuffer *circus;                 ///< circular buffer for that connection
		int opcode;                             ///< last opcode read from network
		wchar_t *plain_buffer;                  ///< plain wchar buffer
		char  *raw_buffer;                      ///< plain char buffer
		
		size_t raw_bytes;                       ///< plaim char buffer read bytes
		struct sockaddr_storage remote_addr;    ///< remote addr struct
		char address_literal[INET6_ADDRSTRLEN]; ///< remote address literal value
		
	public:
		/// Dummy constructor
		Connection();
		/// Real constructor
		Connection(
			int sockfd,                          ///< [in] socket file descriptor
			struct sockaddr_storage remote_addr, ///< [in] remote addr struct
			size_t buffsize                      ///< [in] buffers size
		);
		/// Destructor
		virtual ~Connection();
	public:
		/// Asignment operator
		/// @return Connection refference
		Connection & operator=(
			Connection const &r ///< [in] Right hand expression
		);

	
};


#endif
