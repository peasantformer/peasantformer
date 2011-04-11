#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <Peasantformer/NetworkPrototype.h>


int pn_init() {
	return 0;
}
int pn_getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res) {
	return getaddrinfo(node,service,hints,res);
}
const char * pn_inet_ntop(int af, const void *src, char *dst, size_t size) {
	return inet_ntop(af,src,dst,size);
}
int pn_socket(int domain, int type, int protocol) {
	return socket(domain, type, protocol);
}
int pn_connect(int sockfd, const struct sockaddr *addr, size_t addrlen) {
	return connect(sockfd, addr, addrlen);
}
int pn_listen(int sockfd, int backlog) {
	return listen(sockfd, backlog);
}
int pn_bind(int sockfd, const struct sockaddr *addr, size_t addrlen) {
	return bind (sockfd, addr, addrlen);
}
int pn_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval * time) {
	return select(nfds, readfds, writefds, exceptfds, time);
}
int pn_close(int fd) {
	return close(fd);
}
int pn_quit() {
	return 0;
}

