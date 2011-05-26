#include "World.h"

ServerWorld::ServerWorld(Server *srvr) {
	this->engine = srvr;
	this->circus = new CircularBuffer<wchar_t>(1024);
}

ServerWorld::~ ServerWorld() {
	delete this->circus;
}

void *ServerWorld::game_server(void *raw_data) {
	ServerWorld *world_engine = (ServerWorld *)raw_data;

	struct timeval timeout;
	
	fd_set read_fds;
	FD_ZERO(&read_fds);
	
	char plain_buffer[1024];
	int nbytes;
	
	while (true) {
		
		read_fds = world_engine->engine->network->fd_accepted_socks;
		
		timeout.tv_sec = 0;
		timeout.tv_usec = 1000 * 1000;
		
		pn_select(world_engine->engine->network->fd_accepted_socks_max + 1, &read_fds, NULL, NULL, &timeout);
		for (int i=0; i <= world_engine->engine->network->fd_accepted_socks_max; i++) {
			if (!FD_ISSET(i,&world_engine->engine->network->fd_accepted_socks)) continue;
			if ((nbytes = recv(i,plain_buffer,sizeof(plain_buffer)-1,0)) <= 0) {
				pn_close(i);
				FD_CLR((unsigned int)i,&world_engine->engine->network->fd_accepted_socks);
				printf("[%s] Connection closed by client.\n",world_engine->engine->network->connections[i].address_literal);
				world_engine->engine->network->connections.erase(i);
			}
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
