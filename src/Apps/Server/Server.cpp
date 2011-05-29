#include "Server.h"

/// @file


Server::Server() {
	this->connections = new ServerConnections(this);
	this->network = new ServerNetwork(this);
}

Server::~Server() {
	delete this->connections;
	delete this->network;
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
	
#ifdef _WIN32
	pthread_win32_process_detach_np();
#endif
	return 0;
}
