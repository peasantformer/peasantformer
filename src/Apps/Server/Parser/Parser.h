#ifndef PEASANTFORMER_Apps_Server_Parser
#define PEASANTFORMER_Apps_Server_Parser

class ServerParser;

#include <Apps/Server/Server.h>
#include <Proto/PlainIO/PlainIOHighLevel.h>
#include <Proto/Network/NetworkHighLevel.h>

class ServerParser {
	private:
		Server *engine;
	public:
		ServerParser(Server *srvr);
		~ServerParser();

};

#endif
