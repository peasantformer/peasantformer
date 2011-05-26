#ifndef PEASANTFORMER_Apps_Server_World
#define PEASANTFORMER_Apps_Server_World

class ServerWorld;

#include <Apps/Server/Server.h>
#include <Data/CircularBuffer/CircularBuffer.h>

class ServerWorld {
	private:
		Server *engine;
		CircularBuffer<wchar_t> *circus;
	private:
		static void *game_server(void *raw_data);
	public:
		ServerWorld(Server *srvr);
		~ServerWorld();
	public:
		void setup_world();
};

#endif
