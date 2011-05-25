#ifndef PEASANTFORMER_Apps_Server
#define PEASANTFORMER_Apps_Server

class Server;

#include <pthread.h>
#include <Proto/PlainIO/PlainIOHighLevel.h>
#include <Apps/Server/Network/Network.h>
#include <Apps/Server/Database/Database.h>
#include <Apps/Server/World/World.h>

class Server {
	public:
		ServerNetwork *network;
		ServerDatabase *db;
		ServerWorld *world;
	public:
		Server();
		~Server();
};

#endif
