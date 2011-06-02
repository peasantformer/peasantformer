#include "ThreadConnection.h"

/// @file

void *ThreadConnection::thread_connection_worker(void *data) {
	ThreadConnection *self = (ThreadConnection*)data;
	
	struct sockaddr_storage remote_addr;
	socklen_t addrlen;
	struct timeval timeout;
	
	fd_set read_fds;
	fd_set real_fds;
	
	FD_ZERO(&read_fds);
	FD_ZERO(&real_fds);
	
	int real_fds_max;
	int remote_sock;
	
	
	
	printf("[INFO] Connection thread fired up\n");
	while (self->time_to_exit == false) {
		real_fds_max = self->engine->connections->get_listen_socks_max();	
		real_fds = self->engine->connections->get_listen_socks();
		read_fds = real_fds;
		timeout.tv_sec = 0;
		timeout.tv_usec = 1000 * 1000;
		pn_select(real_fds_max+1,&read_fds,NULL,NULL,&timeout);
		for (int i=0; i <= real_fds_max; i++) {
			if (!FD_ISSET(i,&read_fds)) continue;
			addrlen = sizeof(remote_addr);
			remote_sock = pn_accept(i,(struct sockaddr *)&remote_addr,&addrlen);
			self->engine->connections->add_pending_connection(remote_sock,remote_addr,self->engine->get_buffsize());
			printf("[%s] Incoming connection. Pending...\n",self->engine->connections->get_pending_connection(remote_sock).address_literal);
			pnh_send_str(remote_sock,self->engine->nmsgs->get("hello_kitty"));
		}
		
	}
	printf("[INFO] Connection thread shuted down\n");
	pthread_exit(NULL);
	return NULL;
}

ThreadConnection::ThreadConnection(ServerEngine *srvr) {
	engine = srvr;
	time_to_exit = false;
}

ThreadConnection::~ThreadConnection() {

}

pthread_t ThreadConnection::setup() {
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_create(&thread_id, &attr, thread_connection_worker,this);
	pthread_attr_destroy(&attr);
	return thread_id;
}

void ThreadConnection::desetup() {
	time_to_exit = true;
	pthread_join(thread_id,NULL);
}
