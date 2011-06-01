#include "ThreadLogin.h"

/// @file

void *ThreadLogin::thread_login_worker(void *data) {
	ThreadLogin *self = (ThreadLogin*)data;
	
	socklen_t addrlen;
	struct timeval timeout;
	
	fd_set read_fds;
	fd_set real_fds;
	
	FD_ZERO(&read_fds);
	FD_ZERO(&real_fds);
	
	int real_fds_max;
	
	
	
	printf("[INFO] Login thread fired up\n");
	
	while (self->time_to_exit == false) {
		real_fds_max = self->engine->connections->get_pending_socks_max();	
		real_fds = self->engine->connections->get_pending_socks();
		read_fds = real_fds;
		timeout.tv_sec = 0;
		timeout.tv_usec = 1000 * 1000;
		pn_select(real_fds_max+1,&read_fds,NULL,NULL,&timeout);
		
		for (int i=0; i <= real_fds_max; i++) {
			if (!FD_ISSET(i,&real_fds)) continue;
			ConnectionPending &pconn = self->engine->connections->get_pending_connection(i);
			char buf[1024];
			int raw_bytes;
			pconn.raw_bytes = recv(i,buf,1024,0);
			if (pconn.raw_bytes <= 0) {
				printf("[%s] Pending connection terminated\n",self->engine->connections->get_pending_connection(i).address_literal);
				self->engine->connections->drop_pending_connection(i);
				pn_close(i);
			} else {
			}
		}
		
	}
	
	printf("[INFO] Login thread shuted down\n");
	pthread_exit(NULL);
	return NULL;
}

ThreadLogin::ThreadLogin(ServerEngine *srvr) {
	engine = srvr;
	time_to_exit = false;
}

ThreadLogin::~ThreadLogin() {

}

pthread_t ThreadLogin::setup() {
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_create(&thread_id, &attr, thread_login_worker,this);
	pthread_attr_destroy(&attr);
	return thread_id;
}

void ThreadLogin::desetup() {
	time_to_exit = true;
	pthread_join(thread_id,NULL);
}
