#ifndef PEASANTFORMER_Proto_NetworkHighLevel
#define PEASANTFORMER_Proto_NetworkHighLevel

#include <string.h>

#include "Network.h"

int pnh_set_hints(struct addrinfo *, int, int, int);
void * pnh_get_in_addr(const struct sockaddr *);
int pnh_get_addrinfo_literal(const struct addrinfo *, char *);
int pnh_get_sockaddr_storage_literal(struct sockaddr_storage *, char *);
int pnh_iterate_addrinfo(struct addrinfo *, struct addrinfo *);
int pnh_bind_to_addr_port(const char *, const char *, int, int, char *);
int pnh_send_str(int,const char *);


#endif
