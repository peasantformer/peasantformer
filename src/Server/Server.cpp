#include "Server.h"

int main(int argc, char **argv) {
	ServerEngine engine;
#ifdef _WIN32
	pthread_win32_process_attach_np();
#endif
/*
	char query[] = "select * from users where username = :username";
	char username[] = "admin";
	engine.db_users->prepare(query,sizeof(query));
	engine.db_users->bind_text(1,username,sizeof(username)-1);
	engine.db_users->step();
	while (!engine.db_users->isDone()) {
		printf("%s\n",engine.db_users->column_text(0));
		engine.db_users->step();
	}
*/
	//printf("%s\n",engine.nmsgs->get("last_message"));
	engine.setup_server_on_addr_port("","50600");
	
	printf("Fired up!\n");

#ifdef _WIN32
	pthread_win32_process_detach_np();
#endif	

	return 0;
}

