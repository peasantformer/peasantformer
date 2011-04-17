#include <Peasantformer/Network/NetUnix.h>


int pn_init() {
	return 0;
}
int pn_getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res) {
	return getaddrinfo(node,service,hints,res);
}
void pn_freeaddrinfo(struct addrinfo *res) {
	freeaddrinfo(res);
}
const char * pn_inet_ntop(int af, const void *src, char *dst, socklen_t size) {
	return inet_ntop(af,src,dst,size);
}
int pn_socket(int domain, int type, int protocol) {
	return socket(domain, type, protocol);
}
int pn_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
	return connect(sockfd, addr, addrlen);
}
int pn_listen(int sockfd, int backlog) {
	return listen(sockfd, backlog);
}
int pn_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
	return bind (sockfd, addr, addrlen);
}
int pn_accept(int fd, struct sockaddr *addr, socklen_t *addrlen) {
	return accept(fd,addr,addrlen);
}
int pn_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval * time) {
	return select(nfds, readfds, writefds, exceptfds, time);
}
int pn_setsockopt(int fd, int level, int optname, const void *optval, socklen_t len) {
	return setsockopt(fd,level,optname,optval,len);
}
int pn_socket_invalid(int fd) {
	return (fd < 0);
}
int pn_close(int fd) {
	return close(fd);
}
int pn_quit() {
	return 0;
}
