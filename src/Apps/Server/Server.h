#ifndef PEASANTFORMER_Apps_Server
#define PEASANTFORMER_Apps_Server

/// @file

class Server;

#include <Apps/Server/Connections/Connections.h>
#include <Apps/Server/ConnectionServer/ConnectionServer.h>
#include <Apps/Server/Network/Network.h>

/// Server side meta-class
class Server {
	public:
		const static int buffsize = 1024; ///< default buffers size
	public:
		ServerConnections *connections; ///< Connections.
		ServerNetwork *network;         ///< Network class.
	public:
		/// Main constructor.
		Server();
		/// Destructor.
		~Server();
};

#endif
