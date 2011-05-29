#include "Connection.h"

/// @file

Connection::Connection() {
	this->sockfd = -1;
	this->buffsize = -1;
	this->circus = NULL;
	this->opcode = -1;
	this->plain_buffer = NULL;
	this->address_literal[0] = '\0';
}

Connection::Connection(int sockfd, struct sockaddr_storage remote_addr, size_t buffsize) {
	this->sockfd = sockfd;
	this->buffsize = buffsize;
	this->circus = new CircularBuffer(buffsize);
	this->opcode = -1;
	this->plain_buffer = new wchar_t[buffsize];
	pnh_get_sockaddr_storage_literal(&remote_addr,address_literal);;
}

Connection::~Connection() {
	delete this->circus;
	delete[] this->plain_buffer;
}
