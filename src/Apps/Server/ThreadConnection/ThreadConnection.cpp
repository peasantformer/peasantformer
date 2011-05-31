#include "ThreadConnection.h"

/// @file

void *ThreadConnection::thread_connection_worker(void *data) {
	return NULL;
}

ThreadConnection::ThreadConnection(ServerEngine *srvr) {
	engine = srvr;
	
	time_to_exit = false;
}

ThreadConnection::~ThreadConnection() {
}

void ThreadConnection::setup() {
}

void ThreadConnection::desetup() {
}
