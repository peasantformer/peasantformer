#include "Connections.h"
Connection::Connection() {
	this->address_literal[0] = '\0';
}
Connection::Connection(struct sockaddr_storage remote_addr) {
	this->remote_addr = remote_addr;
	pnh_get_sockaddr_storage_literal(&remote_addr,address_literal);
}

ConnectionPending::ConnectionPending() :
	Connection()
{
	this->isLogged = false;
	this->iterations = 0;
}

ConnectionPending::ConnectionPending(struct sockaddr_storage remote_addr) :
	Connection(remote_addr)
{
	this->isLogged = false;
	this->iterations = 0;
}





ConnectionAccepted::ConnectionAccepted() :
	ConnectionPending()
{}

ConnectionAccepted::ConnectionAccepted(ConnectionPending pend) {
	this->isLogged = pend.isLogged;
	this->remote_addr = pend.remote_addr;
	pnh_get_sockaddr_storage_literal(&remote_addr,address_literal);
}
