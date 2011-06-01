#include "Server.h"

/// @file

size_t ServerEngine::get_buffsize() {
	return buffsize;
}

ServerEngine::ServerEngine() {
	buffsize = 1024;
	
	network = new ServerNetwork(this);
	connections = new ServerConnections(this);
	
	thread_connection = new ThreadConnection(this);
	thread_login = new ThreadLogin(this);
	thread_admin_input = new ThreadAdminInput(this);
}

ServerEngine::~ServerEngine() {
	delete network;
	delete connections;
	
	delete thread_connection;
	delete thread_login;
	delete thread_admin_input;
}

ServerEngine *engine;

void sigint_handler(int sig) {
	printf("Interrupt caught.\n");
	engine->thread_connection->desetup();
	engine->thread_login->desetup();
	engine->thread_admin_input->desetup();
	//exit(1);
}

/// Main function.
///
/// \param [in] argc is a number of arguments
/// \param [in] argv is an argument list
/// @return always zero.
int main(int argc, char **argv) {
#ifdef _WIN32
	pthread_win32_process_attach_np();
#endif
	
	pio_init();
	pn_init();
	
	engine = new ServerEngine;
	
	signal(SIGINT, &sigint_handler);
	printf("[INFO] To interrupt the server use ^C^D sequence.\n");
	
	engine->network->setup("","50600");
	pthread_t thread_connection_t = engine->thread_connection->setup();
	pthread_t thread_login_t = engine->thread_login->setup();
	pthread_t thread_admin_input_t =  engine->thread_admin_input->setup();
	
	pthread_join(thread_connection_t,NULL);
	pthread_join(thread_login_t,NULL);
	pthread_join(thread_admin_input_t,NULL);

	delete engine;
#ifdef _WIN32
	pthread_win32_process_detach_np();
#endif
	return 0;
}
