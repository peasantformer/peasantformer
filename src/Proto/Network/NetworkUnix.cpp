#include "NetworkUnix.h"

int pn_setsockopt(int fd, int level, int optname, const void *optval, socklen_t len) {
	return setsockopt(fd,level,optname,optval,len);
}
int pn_socket_invalid(int fd) {
	return (fd < 0);
}
int pn_close(int fd) {
	return close(fd);
}
