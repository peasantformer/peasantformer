#ifndef PEASANTFORMER_Server
#define PEASANTFORMER_Server


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>	
#include <signal.h>
#include <map>

#include <Peasantformer/Network/Net.h>
#include <Peasantformer/Network/Protocol.h>
#include <Peasantformer/Modules/Modules.h>
#include <Peasantformer/Modules/Interfaces/ObjectInterface.h>
#include <Peasantformer/Data/CircularBuffer.h>
#include <Peasantformer/Server/World.h>



class ServerConnection {
	public:
		struct sockaddr_storage remote_addr;
		char address_literal[INET6_ADDRSTRLEN];
	public:
		ServerConnection() {
			this->address_literal[0] = '\0';
		}
		~ServerConnection() {
		}
		ServerConnection(struct sockaddr_storage remote_addr) {
			this->remote_addr = remote_addr;
			pnh_get_sockaddr_storage_literal(&remote_addr,address_literal);
		}
};



class ServerEngine {
	private:
		int v4_socket;
		int v6_socket;
		fd_set master;
		int fd_max;
		bool ipv4;
		bool ipv6;
		CircularBuffer *buffer;
		std::map<int,ServerConnection> connections;
	public:
		Modules *modules;
		World *world;
		
	public:
		ServerEngine() {
			this->modules = new Modules(
				false // render
				,true // object
			);
			this->buffer = new CircularBuffer(1024);
		}
		~ServerEngine() {
			delete this->modules;
			delete this->buffer;
		}
	public:
		int fire_computor_thread();
		int setup_server_on_addr_port(const char *hostname, const char *port, bool ipv4 = true, bool ipv6 = true);
		static void *listener(void *raw_data);
};

#endif
