#include "Connections.h"

ServerConnections::ServerConnections(Server *srvr) {
	this->engine = srvr;
	FD_ZERO(&this->fd_pending_socks);
	FD_ZERO(&this->fd_accepted_socks);
	FD_ZERO(&this->fd_listen_socks);
	fd_pending_socks_max = -1;
	fd_accepted_socks_max = -1;
	fd_listen_socks_max = -1;
}

ServerConnections::~ServerConnections() {
}

int ServerConnections::add_pending_connection(int remote_sock,struct sockaddr_storage remote_addr) {
	this->connections_pending[remote_sock] = ConnectionPending(remote_addr);
	return 0;
}

void *ServerConnections::connection_server(void *raw_data) {
	ServerConnections *connections_engine = (ServerConnections *)raw_data;
	
	struct sockaddr_storage remote_addr;
	socklen_t addrlen;
	int remote_sock;

	fd_set read_fds;
	FD_ZERO(&read_fds);
	
	while(true) {
		read_fds = connections_engine->fd_listen_socks;
		pn_select(connections_engine->fd_listen_socks_max + 1, &read_fds, NULL, NULL, NULL);
		
		for (int i=0; i <= connections_engine->fd_listen_socks_max; i++) {
			if (!FD_ISSET(i,&read_fds)) continue;
			addrlen = sizeof(remote_addr);
			remote_sock = pn_accept(i,(struct sockaddr *)&remote_addr,&addrlen);
			FD_SET(remote_sock, &connections_engine->fd_pending_socks);
			if (remote_sock > connections_engine->fd_pending_socks_max) {
				connections_engine->fd_pending_socks_max = remote_sock;
			}
			connections_engine->add_pending_connection(remote_sock,remote_addr);
			printf("Incoming connection from %s. Pending...\n",connections_engine->connections_pending[remote_sock].address_literal);
		}
	}
	
	pthread_exit(NULL);
	return NULL;
}
void *ServerConnections::login_server(void *raw_data) {
	ServerConnections *connections_engine = (ServerConnections *)raw_data;
	
	
	
	pthread_exit(NULL);
	return NULL;
}

void ServerConnections::setup_listen_server() {
	int lsv4 = this->engine->network->get_listen_socket_v4();
	int lsv6 = this->engine->network->get_listen_socket_v6();
	
	if (lsv4 > -1) {
		FD_SET(lsv4, &this->fd_listen_socks);
		if (lsv4 > this->fd_listen_socks_max)
			this->fd_listen_socks_max = lsv4;
	}
	
	if (lsv6 > -1) {
		FD_SET(lsv6, &this->fd_listen_socks);
		if (lsv6 > this->fd_listen_socks_max)
			this->fd_listen_socks_max = lsv6;
	}
	
	pthread_t connection_server_thread;
	pthread_t login_server_thread;
	
	pthread_create(&connection_server_thread,NULL,connection_server,this);
	printf("Connection server fired up\n");

	pthread_create(&login_server_thread,NULL,login_server,this);
	printf("Login server fired up\n");
}
