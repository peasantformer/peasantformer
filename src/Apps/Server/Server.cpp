#include "Server.h"

Server::Server() {
	this->network = new ServerNetwork(this);
	this->db = new ServerDatabase(this,"db");
	this->world = new ServerWorld(this);
	this->parser = new ServerParser(this);
	this->connections = new ServerConnections(this);
	this->nmsgs = new Messages("text/network_server_english.txt");
}

Server::~Server() {
	delete this->network;
	delete this->db;
	delete this->world;
	delete this->parser;
	delete this->connections;
	delete this->nmsgs;
}

int main(int argc, char **argv) {
	Server *engine = new Server;
	
#ifdef _WIN32
	pthread_win32_process_attach_np();
#endif


	engine->network->setup_server_on_addr_port("","50600");

	engine->world->setup_world();
	
	
	delete engine;
#ifdef _WIN32
	pthread_win32_process_detach_np();
#endif
	return 0;
}
