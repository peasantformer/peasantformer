#ifndef PEASANTFORMER_Apps_Server_LoginServer
#define PEASANTFORMER_Apps_Server_LoginServer

/// @file

class LoginServer;

#include <pthread.h>
#include <Apps/Server/Server.h>
#include <Proto/Network/NetworkHighLevel.h>

/// Login managing server
class LoginServer {
	private:
		Server *engine; ///< Server instance
	private:
		/// Connection server thread.
		/// @return NULL
		static void *login_server(
			void *data ///< [in] LoginServer instance
		);
	public:
		/// Constructor.
		LoginServer(
			Server *srvr ///< [in] Server instance
		);
		/// Destructor.
		~LoginServer();
	public:
		/// Sets up connection server thread.
		/// @return pthread object
		pthread_t setup();
};

#endif
