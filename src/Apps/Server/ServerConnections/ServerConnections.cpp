#include "ServerConnections.h"

/// @file

ServerConnections::ServerConnections(ServerEngine *srvr) {
	engine = srvr;
	FD_ZERO(&listen_socks);
	FD_ZERO(&pending_socks);
	FD_ZERO(&accepted_socks);
	accepted_socks_max = -1;
	pending_socks_max = -1;
	listen_socks_max = -1;
}

ServerConnections::~ServerConnections() {
}

void ServerConnections::add_listen_socket(int sockfd) {
	FD_SET((unsigned int)sockfd,&listen_socks);
	if (sockfd > listen_socks_max) {
		listen_socks_max = sockfd;
	}
}

void ServerConnections::add_pending_connection(int sockfd, struct sockaddr_storage remote_addr, size_t buffsize) {
	pending_connections[sockfd] = ConnectionPending(sockfd,remote_addr,buffsize);
	FD_SET((unsigned int)sockfd,&pending_socks);
	if (sockfd > pending_socks_max) {
		pending_socks_max = sockfd;
	}
}

void ServerConnections::accept_pending_connection(int sockfd) {
	accepted_connections[sockfd] = ConnectionAccepted(pending_connections[sockfd]);
	pending_connections.erase(sockfd);
	FD_CLR((unsigned int)sockfd,&pending_socks);
	FD_SET((unsigned int)sockfd,&accepted_socks);
	if (sockfd > accepted_socks_max) {
		accepted_socks_max = sockfd;
	}
}

void ServerConnections::drop_pending_connection(int sockfd) {
	pending_connections.erase(sockfd);
	FD_CLR((unsigned int)sockfd,&pending_socks);
}

void ServerConnections::drop_accepted_connection(int sockfd) {
	accepted_connections.erase(sockfd);
	FD_CLR((unsigned int)sockfd,&accepted_socks);
}

ConnectionAccepted & ServerConnections::get_accepted_connection(int i) {
	return accepted_connections[i];
}

ConnectionPending & ServerConnections::get_pending_connection(int i) {
	return pending_connections[i];
}

fd_set ServerConnections::get_accepted_socks() {
	return accepted_socks;
}

fd_set ServerConnections::get_pending_socks() {
	return pending_socks;
}

fd_set ServerConnections::get_listen_socks() {
	return listen_socks;
}

int ServerConnections::get_listen_socks_max() {
	return listen_socks_max;
}

int ServerConnections::get_pending_socks_max() {
	return pending_socks_max;
}

int ServerConnections::get_accepted_socks_max() {
	return accepted_socks_max;
}
