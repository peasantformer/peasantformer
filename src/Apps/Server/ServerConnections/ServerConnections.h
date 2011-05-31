#ifndef PEASANTFORMER_Apps_ServerConnections
#define PEASANTFORMER_Apps_ServerConnections

/// @file

class ServerConnections;

#include <map>

#include <Apps/Server/Server.h>
#include <Data/Connection/ConnectionPending.h>
#include <Data/Connection/ConnectionAccepted.h>

/// Connection managing class
class ServerConnections {
	private:
		Server *engine; ///< Server class instance
		
		std::map<int,ConnectionPending> pending_connections;   ///< pending connections list
		std::map<int,ConnectionAccepted> accepted_connections; ///< accepted connections list
		
		fd_set accepted_socks;  ///< accepted socks list
		int accepted_socks_max; ///< maximum of accepted socks
		
		fd_set pending_socks;   ///< pending socks list
		int pending_socks_max;  ///< maximum of pending socks
		
		fd_set listen_socks;    ///< listen socks list
		int listen_socks_max;   ///< maximum of listen socks
		
	public:
	
		/// Constructor
		///
		/// \param [in] srvr is Server instance
		ServerConnections(Server *srvr);
		
		/// Destructor
		~ServerConnections();
	public:
		
		/// Ads listen socket.
		///
		/// \param [in] sockfd is socket fd
		void add_listen_socket(int sockfd);
		
		/// Adds pending connection.
		///
		/// \param [in] sockfd is socket fd
		/// \param [in] remote_addr is remote addr struct
		/// \param [in] buffsize is buffers length
		void add_pending_connection(int sockfd, struct sockaddr_storage remote_addr, size_t buffsize);
		
		/// Accepts pending connection.
		///
		/// \param [in] sockfd is socket fd
		void accept_pending_connection(int sockfd);
		
		/// Drops pending connection.
		///
		/// \param [in] sockfd is socket fd
		void drop_pending_connection(int sockfd);
		
		/// Drops accepted connection.
		///
		/// \param [in] sockfd is socket fd
		void drop_accepted_connection(int sockfd);
		
		/// Returns accepted connection with given id
		///
		/// \param [in] i is connection id (sockfd)
		/// @return ConnectionAccepted refference
		ConnectionAccepted & get_accepted_connection(int i);
		
		/// Returns pending connection with given id
		///
		/// \param [in] i is connection id (sockfd)
		/// @return ConnectionPending refference
		ConnectionPending & get_pending_connection(int i);
		
		/// Returns accepted socks list
		///
		/// @return fd_set socket list
		fd_set get_accepted_socks();
		
		/// Returns pending socks list
		///
		/// @return fd_set socket list
		fd_set get_pending_socks();
		
		/// Returns listen socks list
		///
		/// @return fd_set socket list
		fd_set get_listen_socks();
		
		/// Returns maximum of listen socks list
		///
		/// @return maximum of sockslist
		int get_listen_socks_max();
		
		/// Returns maximum of pending socks list
		///
		/// @return maximum of sockslist
		int get_pending_socks_max();
		
		/// Returns maximum of accepted socks list
		///
		/// @return maximum of sockslist
		int get_accepted_socks_max();
};

#endif
