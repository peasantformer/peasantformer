#include "World.h"

ServerWorld::ServerWorld(Server *srvr) {
	this->engine = srvr;
	this->circus = new CircularBuffer<wchar_t>(1024);
}

void *ServerWorld::game_server(void *raw_data) {
	ServerWorld *world_engine = (ServerWorld *)raw_data;
	
	fd_set read_fds;
	FD_ZERO(&read_fds);
	
	while (true) {
		pn_select(world_engine->engine->network->fd_accepted_socks_max + 1, &read_fds, NULL, NULL, NULL);
		for (int i=0; i <= world_engine->engine->network->fd_accepted_socks_max; i++) {
			if (!FD_ISSET(i,&world_engine->engine->network->fd_accepted_socks)) continue;
			
		}
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
