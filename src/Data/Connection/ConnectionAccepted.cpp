#include "ConnectionAccepted.h"

ConnectionAccepted::ConnectionAccepted() :
	ConnectionPending()
{}

ConnectionAccepted::ConnectionAccepted(int sockfd, struct sockaddr_storage remote_addr, size_t buffsize) :
	ConnectionPending(sockfd,remote_addr,buffsize)
{}

ConnectionAccepted::ConnectionAccepted(const ConnectionPending &pend) {
	
}
