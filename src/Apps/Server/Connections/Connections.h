#ifndef PEASANTFORMER_Apps_Server_Connections
#define PEASANTFORMER_Apps_Server_Connections

class ServerConnections;

#include <Apps/Server/Server.h>
#include <Proto/PlainIO/PlainIOHighLevel.h>
#include <Proto/Network/NetworkHighLevel.h>

class ServerConnections {
	private:
		Server *engine;
	public:
		ServerConnections(Server *srvr);
		~ServerConnections();

};

#endif
