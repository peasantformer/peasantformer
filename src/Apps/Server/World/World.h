#ifndef PEASANTFORMER_Apps_Server_World
#define PEASANTFORMER_Apps_Server_World

class ServerWorld;

#include <Apps/Server/Server.h>
#include <Data/CircularBuffer/CircularBuffer.h>

#include <wctype.h>

class ServerWorld {
	private:
		Server *engine;
	private:
		static void *game_server(void *raw_data);
	private:
		int scroll_till_opcode(CircularBuffer<wchar_t> &buffer);
	public:
		ServerWorld(Server *srvr);
		~ServerWorld();
	public:
		void setup_world();
};

#endif
