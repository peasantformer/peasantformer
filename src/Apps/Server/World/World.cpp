#include "World.h"

ServerWorld::ServerWorld(Server *srvr) {
	this->engine = srvr;
}

void *ServerWorld::game_server(void *raw_data) {
	while (true) {
	}
	pthread_exit(NULL);
	return NULL;
}

void ServerWorld::setup_world() {
	pthread_t game_server_thread;
	pthread_create(&game_server_thread,NULL,game_server,this);
	printf("Game server fired up\n");

	pthread_join(game_server_thread,NULL);


	//	pthread_exit(NULL);
}
