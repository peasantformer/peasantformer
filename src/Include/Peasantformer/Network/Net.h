#ifndef PEASANTFORMER_Network_Net
#define PEASANTFORMER_Network_Net

#include <stddef.h>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/types.h>
#endif

int pn_init();
int pn_getaddrinfo(const char *, const char *, const struct addrinfo *, struct addrinfo **);
const char * pn_inet_ntop(int, const void *, char *, size_t);
int pn_socket(int, int, int);
int pn_connect(int, const struct sockaddr *, size_t);
int pn_listen(int, int);
int pn_bind(int, const struct sockaddr *, size_t);
int pn_select(int, fd_set *, fd_set *, fd_set *, struct timeval *);
int pn_close(int);
int pn_quit();

#endif
