#ifndef PEASANTFORMER_Apps_Server_WorldInteractorServer
#define PEASANTFORMER_Apps_Server_WorldInteractorServer

/// @file

class WorldInteractorServer;

#include <pthread.h>
#include <Apps/Server/Server.h>
#include <Proto/Network/NetworkHighLevel.h>

/// World interacting server
class WorldInteractorServer {
	private:
		Server *engine; ///< Server instance
	private:
		/// Connection server thread.
		/// @return NULL
		static void *world_interactor_server(
			void *data ///< [in] WorldInteractorServer instance
		);
	public:
		/// Constructor.
		WorldInteractorServer(
			Server *srvr ///< [in] Server instance
		);
		/// Destructor.
		~WorldInteractorServer();
	public:
		/// Sets up connection server thread.
		/// @return pthread object
		pthread_t setup();
};

#endif
