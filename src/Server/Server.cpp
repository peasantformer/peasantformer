#include "Server.h"

int main(int argc, char **argv) {
	ServerEngine engine;
#ifdef _WIN32
	pthread_win32_process_attach_np();
#endif
	
	engine.setup_server_on_addr_port("","50600");

#ifdef _WIN32
	pthread_win32_process_detach_np();
#endif	

	return 0;
}

