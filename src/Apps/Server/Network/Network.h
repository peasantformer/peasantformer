#ifndef PEASANTFORMER_Apps_Server_Network
#define PEASANTFORMER_Apps_Server_Network

class ServerNetwork;

#include <map>

#include <string.h>

#include <Apps/Server/Server.h>
#include <Apps/Server/Connections/Connections.h>
#include <Proto/PlainIO/PlainIOHighLevel.h>
#include <Proto/Network/NetworkHighLevel.h>
#include <Proto/Messages/Messages.h>

class ServerNetwork {
	public:
		fd_set fd_accepted_socks;
		int fd_accepted_socks_max;
	private:
		Server *engine;
		bool do_ipv4;
		bool do_ipv6;
		
		int listen_socket_v4;
		int listen_socket_v6;
		
		fd_set fd_listen_socks;
		int fd_listen_socks_max;
		
		fd_set fd_pending_socks;
		int fd_pending_socks_max;
		
		Messages *nmsgs;
		
		char bind_addres_literal_v4[INET_ADDRSTRLEN];
		char bind_addres_literal_v6[INET6_ADDRSTRLEN];
		
		std::map<int,ConnectionPending> connections_pending;
		std::map<int,ConnectionAccepted> connections;
	private:
		static void *connection_server(void *raw_data);
		static void *login_server(void *raw_data);
	
		int setup_server_on_addr_port_ipv4(pio_string hostname, pio_string port);
		int setup_server_on_addr_port_ipv6(pio_string hostname, pio_string port);
		
		void add_pending_connection(int remote_sock,struct sockaddr_storage  remote_addr);
		void accept_connection(int remote_sock);
	public:
		ServerNetwork(Server *srvr, pio_string msgs_prefix);
	public:
		void setup_server_on_addr_port(pio_string addr, pio_string port);
};

#endif
