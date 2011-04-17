#ifndef PEASANTFORMER_Server
#define PEASANTFORMER_Server

#include <map>
#include <pthread.h>
#include <signal.h>

#include <Proto/Network/NetworkHighLevel.h>
#include <Data/CircularBuffer/CircularBuffer.h>
#include <Data/World/World.h>


class IncomingConnection {
	public:
		struct sockaddr_storage remote_addr;
		char address_literal[INET6_ADDRSTRLEN];
	public:
		IncomingConnection() {
			this->address_literal[0] = '\0';
		}
		~IncomingConnection() {
		}
		IncomingConnection(struct sockaddr_storage remote_addr) {
			this->remote_addr = remote_addr;
			pnh_get_sockaddr_storage_literal(&remote_addr,address_literal);
		}
};

class ServerEngine {
	private:
		int listen_socket_v4;
		int listen_socket_v6;
		fd_set listen_master;
		int fd_max;
		bool do_ipv4;
		bool do_ipv6;
		CircularBuffer *buffer;
		std::map<int,IncomingConnection> connections;
		char bind_addres_literal_v4[INET6_ADDRSTRLEN];
		char bind_addres_literal_v6[INET6_ADDRSTRLEN];
	private:
		World *world;
	public:
		ServerEngine(bool do_ipv4 = true, bool do_ipv6 = true) {
			this->listen_socket_v4 = -1;
			this->listen_socket_v6 = -1;
			FD_ZERO(&this->listen_master);
			this->fd_max = -1;
			this->do_ipv4 = do_ipv4;
			this->do_ipv6 = do_ipv6;
			this->buffer = new CircularBuffer(1024);
			this->bind_addres_literal_v4[0] = '\0';
			this->bind_addres_literal_v6[0] = '\0';
			
			this->world = new World;
		}
		~ServerEngine() {
			delete this->buffer;
			delete this->world;
		}
	public:
		int setup_server_on_addr_port(const char *hostname, const char *port);
		static void *login_server(void *raw_data);
};

#endif
