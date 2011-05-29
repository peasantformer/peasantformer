#include "Connections.h"

/// @file

ServerConnections::ServerConnections(Server *srvr) {
	this->engine = srvr;
	
	FD_ZERO(&this->listen_socks);
	FD_ZERO(&this->pending_socks);
	FD_ZERO(&this->accepted_socks);
	this->accepted_socks_max = -1;
	this->pending_socks_max = -1;
	this->listen_socks_max = -1;
}
ServerConnections::~ServerConnections() {
}

void ServerConnections::add_listen_socket(int sockfd) {
	FD_SET(sockfd,&this->listen_socks);
}

void ServerConnections::add_pending_connection(int sockfd, struct sockaddr_storage remote_addr, size_t buffsize) {
	this->pending_connections[sockfd] = ConnectionPending(sockfd,remote_addr,buffsize);
	FD_SET(sockfd,&this->pending_socks);
	if (sockfd > this->pending_socks_max) {
		this->pending_socks_max = sockfd;
	}
}
void ServerConnections::accept_pending_connection(int sockfd) {
	this->accepted_connections[sockfd] = ConnectionAccepted(this->pending_connections[sockfd]);
	this->pending_connections.erase(sockfd);
	FD_CLR(sockfd,&this->pending_socks);
	FD_SET(sockfd,&this->accepted_socks);
	if (sockfd > this->accepted_socks_max) {
		this->accepted_socks_max = sockfd;
	}
}

void ServerConnections::drop_pending_connection(int sockfd) {
	this->pending_connections.erase(sockfd);
	FD_CLR(sockfd,&this->pending_socks);
}

void ServerConnections::drop_accepted_connection(int sockfd) {
	this->accepted_connections.erase(sockfd);
	FD_CLR(sockfd,&this->accepted_socks);
}

ConnectionAccepted & ServerConnections::operator[] (int i) {
	return this->accepted_connections[i];
}

ConnectionPending & ServerConnections::get_pending_connection (int i) {
	return this->pending_connections[i];
}

fd_set ServerConnections::get_pending_socks_fd_set() {
	return this->pending_socks;
}

fd_set ServerConnections::get_accepted_socks_fd_set() {
	return this->accepted_socks;
}

fd_set ServerConnections::get_listen_socks_fd_set() {
	return this->listen_socks;
}
