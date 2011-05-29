#include "ConnectionServer.h"

void *ConnectionServer::connection_server(void *data) {
	ConnectionServer *connserver = (ConnectionServer*)data;
	
	struct sockaddr_storage remote_addr;
	socklen_t addrlen;
	int remote_sock;

	fd_set read_fds;
	fd_set real_fds;
	
	FD_ZERO(&real_fds);
	FD_ZERO(&read_fds);
	
	while (true) {
		real_fds = connserver->engine->connections->get_listen_socks_fd_set(); 
		read_fds = real_fds;
		pn_select(connserver->engine->connections->get_listen_socks_max() + 1, &read_fds, NULL, NULL, NULL);
		for (int i=0; i <= connserver->engine->connections->get_listen_socks_max(); i++) {
			if (!FD_ISSET(i,&real_fds)) continue;
			addrlen = sizeof(remote_addr);
			remote_sock = pn_accept(i,(struct sockaddr *)&remote_addr,&addrlen);
			connserver->engine->connections->add_pending_connection(remote_sock,remote_addr,connserver->engine->buffsize);
			printf("[%s] Incoming connection. Pending...\n",connserver->engine->connections->get_pending_connection(remote_sock).address_literal);
		}
	}

	pthread_exit(NULL);
	return NULL;
}

ConnectionServer::ConnectionServer(Server *srvr) {
	this->engine = srvr;
}

ConnectionServer::~ConnectionServer() {
}

pthread_t ConnectionServer::setup() {
	pthread_t connection_server_thread;
	pthread_attr_t attr;
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
	
	pthread_create(&connection_server_thread,&attr,connection_server,this);
	pthread_attr_destroy(&attr);
	
	return connection_server_thread;
}
