#ifndef PEASANTFORMER_Apps_Server_ConnectionServer
#define PEASANTFORMER_Apps_Server_ConnectionServer

/// @file

class ConnectionServer;

#include <pthread.h>
#include <Apps/Server/Server.h>
#include <Proto/Network/NetworkHighLevel.h>

/// Connection managing server.
class ConnectionServer {
	private:
		Server *engine; ///< Server instance
	private:
		/// Connection server thread.
		/// @return NULL
		static void *connection_server(
			void *data ///< [in] ConnectionServer instance
		);
	public:
		/// Constructor.
		ConnectionServer(
			Server *srvr ///< Servr instance.
		);
		/// Destructor.
		~ConnectionServer();
	public:
		/// Sets up connection server thread.
		/// @return pthread object
		pthread_t setup();
};

#endif
