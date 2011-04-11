#define _WIN32_WINNT 0x501
#include <winsock2.h>
#include <ws2tcpip.h>




const char *inet_ntop6(struct in6_addr *r, char *buf, size_t size) {
	char tmp[46];
	char *p;
	uint8_t bit,hexbit;
	bool skip_zeros, print_colon, colon_printed, have_printed;
	
	print_colon = false;
	colon_printed = false;
	have_printed = false;
	p = tmp;
	
	for (int i=0; i < 16; i+=2) {
		skip_zeros = true;
		bit = r->s6_addr[i];
		hexbit = bit >> 4;
		if (colon_printed == false && print_colon == true) {
			p += sprintf(p,":");
			if (have_printed == false) {
				p += sprintf(p,":");
			}
			colon_printed = true;
		}
		if (hexbit != 0) {
			skip_zeros = false;
			p += sprintf(p,"%x",hexbit);
			have_printed = true;
		}
		hexbit = bit & 0x0f;
		if (skip_zeros == false || (skip_zeros == true && hexbit != 0)) {
			skip_zeros = false;
			p += sprintf(p,"%x",hexbit);
			have_printed = true;
		}
		bit = r->s6_addr[i+1];
		hexbit = bit >> 4;
		if (skip_zeros == false || (skip_zeros == true && hexbit != 0)) {
			p += sprintf(p,"%d",hexbit);
			have_printed = true;
		}
		hexbit = bit & 0x0f;
		if (skip_zeros == false || (skip_zeros == true && hexbit != 0)) {
			p += sprintf(p,"%x",hexbit);
			if (i < 14) p += sprintf(p,":");
			have_printed = true;
		} else {
			print_colon = true;
		}
	}
	p++;
	if ((p-tmp) > size) return NULL;
	strncpy(buf,tmp,size);
	return buf;
}

const char * inet_ntop(int family, const void *addr, char *buf, size_t size) {
	switch (family) {
		case AF_INET:
			return strncpy(buf,inet_ntoa(*(struct in_addr*)addr),size);
			break;
		case AF_INET6:
			return inet_ntop6((struct in6_addr*)addr,buf,size);
			break;
		default:
			return NULL;
			break;
	}
}




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
	return closesocket(fd);
}
int pn_quit() {
	return 0;
}
