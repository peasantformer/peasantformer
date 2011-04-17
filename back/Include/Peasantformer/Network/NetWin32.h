#ifndef PEASANTFORMER_Network_NetWin32
#define PEASANTFORMER_Network_NetWin32

#include <Peasantformer/Generic/Network.h>

#include <stdio.h>
#include <stdint.h>

#include <Peasantformer/Network/Net.h>

const char *inet_ntop6(struct in6_addr *r, char *buf, size_t size);
const char * inet_ntop(int family, const void *addr, char *buf, size_t size);



#endif
