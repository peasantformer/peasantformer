#include "Connections.h"
Connection::Connection() {
	this->address_literal[0] = '\0';
	this->proto = NULL;
}
Connection::Connection(int sock_fd, struct sockaddr_storage remote_addr, int buffsize, Messages *msgs) {
	this->sock_fd = sock_fd;
	this->remote_addr = remote_addr;
	pnh_get_sockaddr_storage_literal(&remote_addr,address_literal);
	this->proto = new Protocol(buffsize,msgs);
}

ConnectionPending::ConnectionPending() :
	Connection()
{
	this->isLogged = false;
	this->iterations = 0;
}

ConnectionPending::ConnectionPending(int sock_fd, struct sockaddr_storage remote_addr, int buffsize, Messages *msgs) :
	Connection(sock_fd, remote_addr,buffsize,msgs)
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
	this->sock_fd = pend.sock_fd;
	this->proto = pend.proto;
	pnh_get_sockaddr_storage_literal(&remote_addr,address_literal);
}
