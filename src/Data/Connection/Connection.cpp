#include "Connection.h"

/// @file

Connection::Connection() {
	this->sockfd = -1;
	this->buffsize = -1;
	this->circus = NULL;
	this->opcode = -1;
	this->plain_buffer = NULL;
	this->address_literal[0] = '\0';
	this->raw_buffer = NULL;
	this->raw_bytes = -1;
}

Connection::Connection(int sockfd, struct sockaddr_storage remote_addr, size_t buffsize) {
	this->sockfd = sockfd;
	this->buffsize = buffsize;
	this->circus = NULL;
	this->circus = new CircularBuffer(buffsize);
	this->opcode = -1;
	this->plain_buffer = new wchar_t[buffsize];
	this->raw_buffer = new char[buffsize];
	this->raw_bytes = -1;
	this->remote_addr  = remote_addr;
	pnh_get_sockaddr_storage_literal(&remote_addr,this->address_literal);
}
Connection & Connection::operator=(Connection const &r) {
	if (&r == this) return *this;
	this->sockfd = r.sockfd;
	this->buffsize = r.buffsize;
	this->circus = NULL;
	this->circus = new CircularBuffer(buffsize);
	this->opcode = r.opcode;
	this->plain_buffer = new wchar_t[buffsize];
	this->raw_buffer = new char[buffsize];
	this->raw_bytes = -1;
	this->remote_addr = r.remote_addr;
	pnh_get_sockaddr_storage_literal(&remote_addr,this->address_literal);
	return *this;
}

Connection::~Connection() {
	delete this->circus;
	delete[] this->plain_buffer;
	delete[] this->raw_buffer;
}
