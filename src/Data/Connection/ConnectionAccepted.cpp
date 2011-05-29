#include "ConnectionAccepted.h"

/// @file


ConnectionAccepted::ConnectionAccepted() :
	ConnectionPending()
{}

ConnectionAccepted::ConnectionAccepted(int sockfd, struct sockaddr_storage remote_addr, size_t buffsize) :
	ConnectionPending(sockfd,remote_addr,buffsize)
{}

ConnectionAccepted::ConnectionAccepted(ConnectionPending pend) {
	this->sockfd = pend.sockfd;
	this->buffsize = pend.buffsize;
	//this->circus = new CircularBuffer(buffsize);
	this->opcode = pend.opcode;
	//this->plain_buffer = new wchar_t[pend.buffsize];
}
