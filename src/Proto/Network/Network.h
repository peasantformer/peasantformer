#ifndef PEASANTFORMER_Proto_Network
#define PEASANTFORMER_Proto_Network

#ifdef _WIN32

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif

#define _WIN32_WINNT 0x501
#include <winsock2.h>
#include <ws2tcpip.h>

const char *inet_ntop6(struct in6_addr *r, char *buf, socklen_t size);
const char * inet_ntop(int family, const void *addr, char *buf, socklen_t size);

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#endif	

int pn_init();
int pn_getaddrinfo(const char *, const char *, const struct addrinfo *, struct addrinfo **);
void pn_freeaddrinfo(struct addrinfo *res);
const char * pn_inet_ntop(int, const void *, char *, socklen_t);
int pn_socket(int, int, int);
int pn_connect(int, const struct sockaddr *, socklen_t);
int pn_listen(int, int);
int pn_bind(int, const struct sockaddr *, socklen_t);
int pn_accept(int, struct sockaddr *, socklen_t *);
int pn_select(int, fd_set *, fd_set *, fd_set *, struct timeval *);
int pn_socket_invalid(int);
int pn_setsockopt(int,int,int,const void *,socklen_t);
int pn_close(int);
int pn_quit();

#endif
