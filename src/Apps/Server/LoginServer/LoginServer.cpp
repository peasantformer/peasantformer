#include "LoginServer.h"

void *LoginServer::login_server(void *data) {
	LoginServer *loginserver = (LoginServer*)data;
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
