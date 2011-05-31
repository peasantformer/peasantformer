#ifndef PEASANTFORMER_Apps_ServerNetwork
#define PEASANTFORMER_Apps_ServerNetwork

/// @file

class ServerNetwork;

#include <Apps/Server/Server.h>
#include <Data/PString/PString.h>
#include <Proto/Network/NetworkHighLevel.h>

/// Networking class
class ServerNetwork {
	private:
		Server *engine; ///< istance of server engine
	
		int ipv4_listen_socket; ///< ipv4 listening socket
		int ipv6_listen_socket; ///< ipv6 listening socket
		char bind_address_ipv4_literal[INET_ADDRSTRLEN];  ///< bind ipv4 address literal
		char bind_address_ipv6_literal[INET6_ADDRSTRLEN]; ///< bind ipv6 address literal
	private:
		
		/// Setups server on ipv4 address and port
		///
		/// \param [in] address is address to bind on
		/// \param [in] port is port to bind on
		/// @return < 0 on error
		int setup_ipv4_listener(PString address, PString port);
		
		/// Setups server on ipv6 address and port
		///
		/// \param [in] address is address to bind on
		/// \param [in] port is port to bind on
		/// @return < 0 on error
		int setup_ipv6_listener(PString address, PString port);
	public:
		
		/// Default constructor
		///
		/// \param [in] srvr is an istance of server class
		ServerNetwork(Server *srvr);
		
		/// Default destructor
		~ServerNetwork();
	public:
		
		/// Sets up listening server on given address and port
		///
		/// \param [in] address to bind on
		/// \param [in] port to bind on
		void setup(PString address, PString port);
		
		/// Get ipv4 listening socket
		///
		/// @return socket fd
		int get_ipv4_socket();
		
		/// Get ipv6 listening socket
		///
		/// @return socket fd
		int get_ipv6_socket();
};

#endif
