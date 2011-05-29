#include "LoginServer.h"

void *LoginServer::login_server(void *data) {
	LoginServer *loginserver = (LoginServer*)data;
	struct timeval timeout;

	fd_set read_fds;
	fd_set real_fds;
	FD_ZERO(&read_fds);
	FD_ZERO(&real_fds);
	
	while (true) {
		real_fds = loginserver->engine->connections->get_pending_socks_fd_set(); 
		read_fds = real_fds;
		timeout.tv_sec = 0;
		timeout.tv_usec = 1000 * 1000;
		pn_select(loginserver->engine->connections->get_pending_socks_max() + 1, &read_fds, NULL, NULL, &timeout);
		for (int i=0; i <= loginserver->engine->connections->get_pending_socks_max(); i++) {
			if (!FD_ISSET(i,&real_fds)) continue;
			ConnectionPending &pconn = loginserver->engine->connections->get_pending_connection(i);
			pconn.raw_bytes = recv(i,pconn.raw_buffer,pconn.buffsize-1,0);
			if (pconn.raw_bytes <= 0) {
				loginserver->engine->connections->drop_pending_connection(i);
				printf("[%s] Pending connection terminated\n",pconn.address_literal);
				pn_close(i);
			} else {
				pconn.raw_buffer[pconn.raw_bytes] = '\0';
				pconn.circus->write(pconn.raw_buffer,pconn.raw_bytes + 1);
			}
		}
	}
	
	pthread_exit(NULL);
	return NULL;
}

LoginServer::LoginServer(Server *srvr) {
	this->engine = srvr;
}

LoginServer::~LoginServer() {
}

pthread_t LoginServer::setup() {
	pthread_t login_server_thread;
	pthread_attr_t attr;
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
	
	pthread_create(&login_server_thread,&attr,login_server,this);
	pthread_attr_destroy(&attr);
	
	return login_server_thread;
}
