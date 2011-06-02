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
	
	nmsgs = new Messages("text/network_server_english.txt");
}

ServerEngine::~ServerEngine() {
	delete network;
	delete connections;
	delete nmsgs;
	
	delete thread_connection;
	delete thread_login;
	delete thread_admin_input;
}

ServerEngine *engine; ///< ServerEngine instance

/// Interuption signal handler
///
/// \param [in] sig is a signal number
void sigint_handler(int sig) {
	printf("Interrupt caught.\n");
	engine->connections->disconnect_all();
		
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
	

	printf("[INFO] To interrupt the server correctly use ^D^C sequence.\n");
	printf("[INFO] ^D is needed to stop AdminInput thread from reading stdin\n");
	printf("[INFO] ^C is needed to actually interrupt rest of the server\n");
	//printf("\n");

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
