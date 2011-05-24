#ifndef PEASANTFORMER_Server
#define PEASANTFORMER_Server

#include <map>
#include <string>
#include <iostream>
#include <sstream>
#include <pthread.h>
#include <signal.h>

#include <Proto/Network/NetworkHighLevel.h>
#include <Proto/PlainIO/PlainIOHighLevel.h>
#include <Data/CircularBuffer/CircularBuffer.h>
#include <Data/World/World.h>
#include <Database/DBL/DBL.h>
#include <Comm/NetworkMessages/Messages.h>

using namespace std;

class Connection {
	public:
		struct sockaddr_storage remote_addr;
		char address_literal[INET6_ADDRSTRLEN];
	public:
		Connection() {
			this->address_literal[0] = '\0';
		}
		Connection(struct sockaddr_storage remote_addr) {
			this->remote_addr = remote_addr;
			pnh_get_sockaddr_storage_literal(&remote_addr,address_literal);
		}
		virtual ~Connection() {}
};

class PendingConnection : public Connection {
	public:
		bool isLogged;
		int iterations;
	public:
		PendingConnection() : Connection() {
			this->init();
		}
		PendingConnection(struct sockaddr_storage remote_addr) :
			Connection(remote_addr)
		{
			this->init();
		}
	public:
		void init() {
			this->isLogged = false;
			this->iterations = 0;
		}
};

class EstablishedConnection : public PendingConnection {
	public:
		EstablishedConnection() {
		}
		EstablishedConnection(struct sockaddr_storage remote_addr) :
			PendingConnection(remote_addr)
		{
		}
};

class ServerEngine {
	private:
		int listen_socket_v4;
		int listen_socket_v6;

		fd_set listen_master;
		int fd_master_max;
		
		fd_set accepted_socks;
		int fd_accpeted_max;
		
		fd_set pending_socks;
		int fd_pending_max;


		bool do_ipv4;
		bool do_ipv6;
		std::map<int,PendingConnection *> pending_connections;
		char bind_addres_literal_v4[INET6_ADDRSTRLEN];
		char bind_addres_literal_v6[INET6_ADDRSTRLEN];
	public:		
		DataBase *db;
		NetworkMessages *nmsgs;
	private:
		World *world;
	public:
		ServerEngine(bool do_ipv4 = true, bool do_ipv6 = true) {
			this->listen_socket_v4 = -1;
			this->listen_socket_v6 = -1;
			FD_ZERO(&this->accepted_socks);
			FD_ZERO(&this->listen_master);
			FD_ZERO(&this->pending_socks);
			this->fd_master_max = -1;
			this->fd_accpeted_max = -1;
			this->fd_pending_max = -1;
			this->do_ipv4 = do_ipv4;
			this->do_ipv6 = do_ipv6;
			this->bind_addres_literal_v4[0] = '\0';
			this->bind_addres_literal_v6[0] = '\0';
			
			this->world = new World;
			this->db = new DataBase("db");
			this->nmsgs = new NetworkMessages("text/network_server_english.txt");

			pio_init();
		}
		~ServerEngine() {
			std::map<int,PendingConnection *>::iterator it;
			for (it = pending_connections.begin(); it != pending_connections.end(); it++) {
				delete it->second;
			}
			delete this->world;
			delete this->db;
		}
	public:
		int setup_server_on_addr_port(const char *hostname, const char *port);
		int setup_server_on_addr_port_ipv4(const char *hostname, const char *raw_hostname, const char *port);
		int setup_server_on_addr_port_ipv6(const char *hostname, const char *raw_hostname, const char *port);
		static void *connection_server(void *raw_data);
		static void *login_server(void *raw_data);
		static void *game_server(void *raw_data);

};

#endif
