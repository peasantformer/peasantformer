#include "ConnectionPending.h"

/// @file

ConnectionPending::ConnectionPending() :
	Connection()
{}

ConnectionPending::ConnectionPending(int sockfd, struct sockaddr_storage remote_addr, size_t buffsize) :
	Connection(sockfd,remote_addr,buffsize)
{}

