#ifndef PEASANTFORMER_Apps_Server_Connections
#define PEASANTFORMER_Apps_Server_Connections

#include <Proto/Network/NetworkHighLevel.h>
#include <Proto/PlainIO/PlainIOHighLevel.h>
#include <Data/CircularBuffer/CircularBuffer.h>

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
		virtual ~ConnectionPending() {}
};

class ConnectionAccepted : public ConnectionPending {
	public:
		pio_string username;
		pio_string password_hash;
		pio_string nickname;
		CircularBuffer<wchar_t> *circus;
	public:
		ConnectionAccepted();
		ConnectionAccepted(ConnectionPending);
};

#endif
