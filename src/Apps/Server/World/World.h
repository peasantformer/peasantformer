#ifndef PEASANTFORMER_Apps_Server_World
#define PEASANTFORMER_Apps_Server_World

class ServerWorld;

#include <Apps/Server/Server.h>

class ServerWorld {
	private:
		Server *engine;
	private:
		static void *game_server(void *raw_data);
	public:
		ServerWorld(Server *srvr);
	public:
		void setup_world();
};

#endif
