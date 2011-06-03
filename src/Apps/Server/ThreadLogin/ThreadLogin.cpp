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
	
	Messages *msgs = self->engine->nmsgs;
	
	
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
			pconn.raw_bytes = recv(i,pconn.raw_buffer,self->engine->get_buffsize()-1,0);
			if (pconn.raw_bytes <= 0) {
				printf("[%s] Pending connection terminated\n",self->engine->connections->get_pending_connection(i).address_literal);
				self->engine->connections->drop_pending_connection(i);
				pn_close(i);
			} else {
				pconn.raw_buffer[pconn.raw_bytes] = '\0';
				pconn.parse_init();
				
				if (pconn.opcode != 2) {
					pconn.plain_buffer.filter_control();
					pnh_send_fstr(i,self->engine->get_buffsize(),msgs->get("invalid_query").c_str(),pconn.plain_buffer.c_str());
					continue;
				}
				wchar_t nw_login[33] = {0};
				wchar_t nw_password_hash[33] = {0};
				swscanf(pconn.plain_buffer.w_str(),msgs->get("login_message").w_str(),nw_password_hash,nw_login);
				std::vector< Hash<PString> > auth_data = self->engine->db->db_users_lookup_user(nw_login,nw_password_hash);
				if (auth_data.size() != 1) {
					pnh_send_str(i,msgs->get_num("wrong_auth").c_str());
					if (auth_data.size() > 1) {
						printf(msgs->get("auth_duplicate").c_str(),PString(nw_login).c_str());
					}
					continue;
				}
				
				pnh_send_str(i,msgs->get_num("successful_auth").c_str());
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
