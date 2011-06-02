#ifndef PEASANTFORMER_Data_Connection
#define PEASANTFORMER_Data_Connection

/// @file

#include <string.h>

#include <Data/CircularBuffer/CircularBuffer.h>
#include <Data/PString/PString.h>
#include <Proto/PlainIO/PlainIO.h>
#include <Proto/Network/NetworkHighLevel.h>

/// Generic connection class
class Connection {
	public:
		int sockfd;                             ///< socket fd
		size_t buffsize;                        ///< buffers size
		char *raw_buffer;                       ///< raw buffer
		PString plain_buffer;                   ///< plain buffer
		CircularBuffer *circus;                 ///< circular buffer
		struct sockaddr_storage remote_addr;    ///< remote addr struct
		char address_literal[INET6_ADDRSTRLEN]; ///< remote addr literal value
		int raw_bytes;                          ///< raw bytes read
		int opcode;                             ///< last opcode
	private:
		/// Initializes same thing for all constructors
		void init();
	public:
		
		/// Dummy constructor
		Connection();
		
		/// Real constructor
		///
		/// \param [in] sockfd is a socket fd
		/// \param [in] remote_addr is remote addr struct
		/// \param [in] buffsize is buffers size
		Connection(int sockfd, struct sockaddr_storage remote_addr, size_t buffsize);
		
		/// Virtual destructor
		virtual ~Connection();
	public:
		
		/// Asignment operator.
		///
		/// \param [in] r is right-hand expression
		/// @return Connection refference
		Connection & operator=(Connection const &r);
	public:
	
		/// Parses raw buffer initialy.
		///
		/// Stream seekd to opcode and opcode parsed in numerical form
		/// into opcode variable.
		void parse_init();
};

#endif
