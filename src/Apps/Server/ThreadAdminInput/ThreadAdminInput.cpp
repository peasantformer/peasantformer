#include "ThreadAdminInput.h"

void *ThreadAdminInput::thread_admin_input_worker(void *data) {
	ThreadAdminInput *self = (ThreadAdminInput*)data;
	fd_set real_fds;
	fd_set read_fds;
	FD_ZERO(&real_fds);
	FD_ZERO(&read_fds);
	FD_SET(0,&real_fds);
	
	char plain_buffer[1024];
	
	printf("[INFO] Admin input server fired up\n");
	
	while (self->time_to_exit == false) {
		read_fds = real_fds;
		pn_select(1, &read_fds, NULL, NULL, NULL);
		int retcode = read(0,plain_buffer,1023);
		if (retcode <= 0) break;
		plain_buffer[retcode] = '\0';
		printf("%s\n",plain_buffer);
	}
	printf("[INFO] Admin input server shutted down\n");
	pthread_exit(NULL);
	return NULL;
}

ThreadAdminInput::ThreadAdminInput(ServerEngine *srvr) {
	engine = srvr;
	time_to_exit = false;
}

ThreadAdminInput::~ThreadAdminInput() {
}

pthread_t ThreadAdminInput::setup() {
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	pthread_create(&thread_id, &attr, thread_admin_input_worker,this);
	pthread_attr_destroy(&attr);
	return thread_id;
}

void ThreadAdminInput::desetup() {
	time_to_exit = true;
	pthread_join(thread_id,NULL);
}
