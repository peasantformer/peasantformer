#ifndef PEASANTFORMER_Apps_Server_Network
#define PEASANTFORMER_Apps_Server_Network

/// @file

class ServerNetwork;

#include <Apps/Server/Server.h>
#include <Proto/Network/NetworkHighLevel.h>
#include <Proto/PlainIO/PlainIOHighLevel.h>

/// Server-side networking class
class ServerNetwork {
	private:
		Server *engine; ///< server class instance
	
		int ipv4_socket; ///< ipv4 socket file descriptor
		int ipv6_socket; ///< ipv6socket file descriptor
		char ipv4_addr_literal[INET_ADDRSTRLEN];  ///< bind ipv4 address literal
		char ipv6_addr_literal[INET6_ADDRSTRLEN]; ///< bind ipv6 address literal
	private:
		/// Sets up ipv4 listening server on specified address and port.
		/// @return <0 on error
		int setup_server_on_ipv4_address_port(
			pio_string address, ///< [in] address to bind
			pio_string port     ///< [in] port to bind
		);
		/// Sets up ipv6 listening server on specified address and port.
		/// @return <0 on error
		int setup_server_on_ipv6_address_port(
			pio_string address, ///< [in] address to bind
			pio_string port     ///< [in] port to bind
		);
	public:
		/// Constructor
		ServerNetwork(
			Server *srvr // Server class instance.
		);
		/// Destructor
		~ServerNetwork();
	public:
		/// Sets up listening server on specified address and port
		void setup_server_on_address_port(
			pio_string address, ///< [in] address to bind
			pio_string port     ///< [in] port to bind
		);
		/// Get ipv4 listen socket
		/// @return socket file descriptor
		int get_ipv4_socket();
		/// Get ipv6 listen socket
		/// @return socket file descriptor
		int get_ipv6_socket();
};

#endif
