#ifndef PEASANTFORMER_Apps_Server_Network
#define PEASANTFORMER_Apps_Server_Network

class ServerNetwork;

#include <Apps/Server/Server.h>
#include <Proto/PlainIO/PlainIOHighLevel.h>
#include <Proto/Network/NetworkHighLevel.h>
#include <Proto/Messages/Messages.h>

class ServerNetwork {
	private:
		Server *engine;
		bool do_ipv4;
		bool do_ipv6;
		
		int listen_socket_v4;
		int listen_socket_v6;
		
		fd_set fd_listen_socks;
		int fd_listen_socks_max;
		
		fd_set fd_accepted_socks;
		int fd_accepted_socks_max;
		
		fd_set fd_pending_socks;
		int fd_pending_socks_max;
		
		fd_set fd_est_socks;
		int fd_est_socks_max;
		
		Messages *nmsgs;
		
		char bind_addres_literal_v4[INET6_ADDRSTRLEN];
		char bind_addres_literal_v6[INET6_ADDRSTRLEN];
	private:
		static void *connection_server(void *raw_data);
		static void *login_server(void *raw_data);
	
		int setup_server_on_addr_port_ipv4(pio_string hostname, pio_string port);
		int setup_server_on_addr_port_ipv6(pio_string hostname, pio_string port);
	public:
		ServerNetwork(Server *srvr, pio_string msgs_prefix);
	public:
		void setup_server_on_addr_port(pio_string addr, pio_string port);
};

#endif
