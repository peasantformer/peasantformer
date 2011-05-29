#include "WorldInteractorServer.h"

void *WorldInteractorServer::world_interactor_server(void *data) {
	WorldInteractorServer *worldinteractorserver = (WorldInteractorServer*)data;
	pthread_exit(NULL);
	return NULL;
}

WorldInteractorServer::WorldInteractorServer(Server *srvr) {
	this->engine = srvr;
}

WorldInteractorServer::~WorldInteractorServer() {
}

pthread_t WorldInteractorServer::setup() {
	pthread_t world_interactor_thread;
	pthread_attr_t attr;
	
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
	
	pthread_create(&world_interactor_thread,&attr,world_interactor_server,this);
	pthread_attr_destroy(&attr);
	
	return world_interactor_thread;
}
