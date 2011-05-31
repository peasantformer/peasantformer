#ifndef PEASANTFORMER_Apps_Server
#define PEASANTFORMER_Apps_Server

/// @file

class ServerEngine;

#include <Apps/Server/ServerNetwork/ServerNetwork.h>
#include <Apps/Server/ServerConnections/ServerConnections.h>

#include <Apps/Server/ThreadConnection/ThreadConnection.h>

#include <Proto/Network/Network.h>
#include <Proto/PlainIO/PlainIO.h>
#include <Data/PString/PString.h>

/// Server engine class
class ServerEngine {
	private:
	public:
		/// Constructor
		ServerEngine();
		/// Destructor
		~ServerEngine();
	public:
		ServerNetwork *network;         ///< networking class
		ServerConnections *connections; ///< connections class
		
		ThreadConnection *thread_connection; ///< connection handler thread
};


#endif
