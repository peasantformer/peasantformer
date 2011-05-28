#ifndef PEASANTFORMER_Apps_Server_Connections
#define PEASANTFORMER_Apps_Server_Connections

class ServerConnections;

#include <map>

#include <Apps/Server/Server.h>
#include <Proto/PlainIO/PlainIOHighLevel.h>
#include <Proto/Network/NetworkHighLevel.h>
#include <Data/Connections/Connections.h>

class ServerConnections {
	private:
		Server *engine;
		std::map<int,ConnectionPending> connections_pending;
		std::map<int,ConnectionAccepted> connections_accepted;
		
		fd_set fd_pending_socks;
		int fd_pending_socks_max;
		
		fd_set fd_accepted_socks;
		int fd_accepted_socks_max;
		
		fd_set fd_listen_socks;
		int fd_listen_socks_max;
	private:
		static void *connection_server(void *raw_data);
		static void *login_server(void *raw_data);
		int add_pending_connection(int i,struct sockaddr_storage remote_addr);
		int accept_pending_connection(int i);
	public:
		ServerConnections(Server *srvr);
		~ServerConnections();
	public:
		ConnectionAccepted & operator[](int i);
	public:
		void setup_listen_server();
		fd_set get_accepted_socks();
};

#endif
