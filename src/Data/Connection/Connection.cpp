#include "Connection.h"

void Connection::init() {
	sockfd = -1;
	buffsize = 0;
	circus = NULL;
	raw_buffer = NULL;
	plain_buffer = NULL;
	address_literal[0] = '\0';
	raw_bytes = -1;
	opcode = 0;
}

Connection::Connection() {
	init();
}
Connection::Connection(int sockfd, struct sockaddr_storage remote_addr, size_t buffsize) {
	init();
	this->sockfd = sockfd;
	this->remote_addr = remote_addr;
	this->buffsize = buffsize;
	this->raw_buffer = new char[buffsize];
	this->plain_buffer = new wchar_t[buffsize];
	circus = new CircularBuffer(buffsize);
	pnh_get_sockaddr_storage_literal(&remote_addr,address_literal);
}

Connection::~Connection() {
	delete[] raw_buffer;
	delete[] plain_buffer;
	delete circus;
}

Connection & Connection::operator=(Connection const &r) {
	if (&r == this) return *this;
	delete[] raw_buffer;
	delete[] plain_buffer;
	delete circus;
	init();
	sockfd = r.sockfd;
	buffsize = r.buffsize;
	remote_addr = r.remote_addr;
	raw_buffer = new char[buffsize];
	plain_buffer = new wchar_t[buffsize];
	circus = new CircularBuffer(buffsize);
	pnh_get_sockaddr_storage_literal(&remote_addr,address_literal);
	return *this;
}
