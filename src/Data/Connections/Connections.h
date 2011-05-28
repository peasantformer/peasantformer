#ifndef PEASANTFORMER_Data_Connections
#define PEASANTFORMER_Data_Connections

#include <Proto/Network/NetworkHighLevel.h>
#include <Proto/PlainIO/PlainIOHighLevel.h>


class Connection {
	public:
		struct sockaddr_storage remote_addr;
		char address_literal[INET6_ADDRSTRLEN];
	public:
		Connection();
		Connection(struct sockaddr_storage remote_addr);
		virtual ~Connection() {}
};

class ConnectionPending : public Connection {
	friend class ConnectionAccepted;
	public:
		bool isLogged;
		int iterations;

	public:
		ConnectionPending();
		ConnectionPending(struct sockaddr_storage remote_addr);
		virtual ~ConnectionPending() {
			delete this->circus;
		}
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
