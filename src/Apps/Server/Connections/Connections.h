#ifndef PEASANTFORMER_Apps_Server_Connections
#define PEASANTFORMER_Apps_Server_Connections

/// @file
#include <map>

class ServerConnections;

#include <Apps/Server/Server.h>
#include <Data/Connection/ConnectionPending.h>
#include <Data/Connection/ConnectionAccepted.h>

/// Connections management class.
class ServerConnections {
	private:
		Server *engine; ///< server class instance
	
		std::map<int,ConnectionPending> pending_connections;   ///< pending connections storage
		std::map<int,ConnectionAccepted> accepted_connections; ///< accepted connections storage

		fd_set accepted_socks;  ///< accepted_socks fd_set
		int accepted_socks_max; ///< accepted socks maximum descriptor value

		fd_set pending_socks;   ///< pending socks fd_set
		int pending_socks_max;  ///< pending socks maximum descriptor value

		fd_set listen_socks;    ///< listen socks fd_set
		int listen_socks_max;   ///< listen socks maximum descriptor value
	public:
		/// Constructor.
		ServerConnections(
			Server *srvr ///< [in] Server class instance.
		);
		/// Destructor.
		~ServerConnections();
	public:
		/// Adds listen socket
		void add_listen_socket(
			unsigned int sockfd ///< [in] socket file descriptor
		);
		/// Adds pending connection to pending connection list
		void add_pending_connection(
			unsigned int sockfd,                 ///< [in] socket file descriptor
			struct sockaddr_storage remote_addr, ///< [in]	 remote addr struct
			size_t buffsize                      ///< [in] buffer size
		);
		/// Accepts pending conection and promotes it to "accepted" state
		void accept_pending_connection(
			unsigned int sockfd ///< [in] socket file descriptor
		);
		/// Drops existing pending connection
		void drop_pending_connection(
			unsigned int sockfd ///< [in] socket file descriptor
		);
		/// Drops existing accepted connection
		void drop_accepted_connection(
			unsigned int sockfd ///< [in] socket file descriptor
		);
		/// Returns accepted connection
		/// @return ConnectionAccepted refference
		ConnectionAccepted & operator[] (
			int i ///< [in] Index of accepted connection
		);
		
		/// Returns pending connection
		/// @return ConnectionPending refference
		ConnectionPending & get_pending_connection (
			int i ///< [in] Index of pending connection
		);

		/// @return fd_set of accepted socks
		fd_set get_accepted_socks_fd_set();
		
		/// @return fd_set of pending socks
		fd_set get_pending_socks_fd_set();
		
		/// @return fd_set of listen socks
		fd_set get_listen_socks_fd_set();
		
		/// @return listen_sock max
		int get_listen_socks_max();
		
		/// @return pending_sock max
		int get_pending_socks_max();
		
		/// @return accepted_sock max
		int get_accepted_socks_max();
};

#endif
