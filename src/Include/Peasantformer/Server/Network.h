#ifndef PEASANTFORMER_Server_Network
#define PEASANTFORMER_Server_Network

#include <stdio.h>
#include <stdlib.h>
#include <map>

#include <Peasantformer/Network/Net.h>
#include <Peasantformer/Network/Protocol.h>
#include <Peasantformer/Generic/Network.h>
#include <Peasantformer/Generic/Threads.h>

#include <Peasantformer/Data/CircularBuffer.h>



class ServerConnection {
	public:
		struct sockaddr_storage remote_addr;
		char address_literal[INET6_ADDRSTRLEN];
	public:
		ServerConnection() {
			this->address_literal[0] = '\0';
		}
		ServerConnection(struct sockaddr_storage remote_addr) {
			this->remote_addr = remote_addr;
			pnh_get_sockaddr_storage_literal(&remote_addr,address_literal);
		}
};

class ServerNetwork {
	private:
		int v4_socket;
		int v6_socket;
		CircularBuffer *buffer;
		std::map<int,ServerConnection> connections;
	private:
	public:
		ServerNetwork() {
			this->buffer = new CircularBuffer(1024);
		}
	public:
		int setup_server_on_addr_port(const char *hostname, const char *port, bool ipv4 = true, bool ipv6 = true);
		
		
};



#endif
