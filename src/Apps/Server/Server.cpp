#include "Server.h"

/// @file

ServerEngine::ServerEngine() {
	network = new ServerNetwork(this);
	connections = new ServerConnections(this);
	
	thread_connection = new ThreadConnection(this);
};

ServerEngine::~ServerEngine() {
	delete network;
	delete connections;
	
	delete thread_connection;
};

/// Main function.
///
/// \param [in] argc is a number of arguments
/// \param [in] argv is an argument list
/// @return always zero.
int main(int argc, char **argv) {
	pio_init();
	pn_init();
	ServerEngine engine;
	
	engine.network->setup("","50600");
	return 0;
}
