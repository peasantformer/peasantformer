#ifndef PEASANTFORMER_Apps_Server_Protocol
#define PEASANTFORMER_Apps_Server_Protocol

class ServerProtocol;

#include <Apps/Server/Server.h>
#include <Proto/PlainIO/PlainIOHighLevel.h>
#include <Proto/Network/NetworkHighLevel.h>
#include <Proto/Messages/Messages.h>

class ServerProtocol {
	private:
		Server *engine;
		Messages *nmsgs;
	public:
		ServerProtocol(Server *srvr,pio_string prefix);
		~ServerProtocol();
};

#endif
