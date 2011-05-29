#include "ConnectionServer.h"

void *ConnectionServer::connection_server(void *data) {
	pthread_exit(NULL);
	return NULL;
}

ConnectionServer::ConnectionServer(Server *srvr) {
}

ConnectionServer::~ConnectionServer() {
}

pthread_t ConnectionServer::setup() {
	pthread_t connection_server_thread;
	pthread_attr_t attr;
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
	
	pthread_create(&connection_server_thread,&attr,connection_server,this);
	pthread_attr_destroy(&attr);
	
	return connection_server_thread;
}
