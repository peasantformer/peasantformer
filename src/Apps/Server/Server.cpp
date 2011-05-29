#include "Server.h"

/// @file


Server::Server() {
	this->connections = new ServerConnections(this);
	this->network = new ServerNetwork(this);
	this->connection_server = new ConnectionServer(this);
	this->login_server = new LoginServer(this);
	this->winteract_server = new WorldInteractorServer(this);
}

Server::~Server() {
	delete this->connections;
	delete this->network;
	delete this->connection_server;
	delete this->login_server;
	delete this->winteract_server;
}


/// Main function. 
/// @return zero, ends never
int main(
	int argc,   ///< [in] arguments count
	char **argv ///< [in] arguments list
) {
#ifdef _WIN32
	pthread_win32_process_attach_np();
#endif

	Server *engine = new Server;
	engine->network->setup("","50600");
	pthread_t connection_server_thread = engine->connection_server->setup();
	pthread_t login_server_thread = engine->login_server->setup();
	pthread_t winteract_server_thread = engine->winteract_server->setup();
	
	pthread_join(connection_server_thread,NULL);
	pthread_join(login_server_thread,NULL);
	pthread_join(winteract_server_thread,NULL);
	
	delete engine;
#ifdef _WIN32
	pthread_win32_process_detach_np();
#endif
	return 0;
}
