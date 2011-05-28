#ifndef PEASANTFORMER_Data_Connections
#define PEASANTFORMER_Data_Connections

#include <Proto/Network/NetworkHighLevel.h>
#include <Proto/PlainIO/PlainIOHighLevel.h>
#include <Proto/Protocol/Protocol.h>
#include <Proto/Messages/Messages.h>



class Connection {
	public:
		struct sockaddr_storage remote_addr;
		char address_literal[INET6_ADDRSTRLEN];
		int sock_fd;
		Protocol *proto;
	public:
		Connection();
		Connection(int sock_fd, struct sockaddr_storage remote_addr, int buffsize,Messages *msgs);
		virtual ~Connection() {}
};

class ConnectionPending : public Connection {
	friend class ConnectionAccepted;
	public:
		bool isLogged;
		int iterations;

	public:
		ConnectionPending();
		ConnectionPending(int sock_fd, struct sockaddr_storage remote_addr, int buffsize, Messages *msgs);
		virtual ~ConnectionPending() {}
};

class ConnectionAccepted : public ConnectionPending {
	public:
		pio_string username;
		pio_string password_hash;
		pio_string nickname;
	public:
		ConnectionAccepted();
		ConnectionAccepted(ConnectionPending);
};

#endif
