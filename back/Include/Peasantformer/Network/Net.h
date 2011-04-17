#ifndef PEASANTFORMER_Network_Net
#define PEASANTFORMER_Network_Net

#include <stddef.h>

#include <Peasantformer/Generic/Network.h>

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



int pnh_set_hints(struct addrinfo *, int, int, int);
void * pnh_get_in_addr(const struct sockaddr *);
int pnh_get_addrinfo_literal(const struct addrinfo *, char *);
int pnh_get_sockaddr_storage_literal(struct sockaddr_storage *, char *);
int pnh_iterate_addrinfo(struct addrinfo *, struct addrinfo *);
int pnh_bind_to_addr_port(const char *, const char *, int, int, struct addrinfo *, struct addrinfo **);

#endif
