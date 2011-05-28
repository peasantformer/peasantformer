#ifndef PEASANTFORMER_Proto_NetworkWin32
#define PEASANTFORMER_Proto_NetworkWin32

/// @file

#include <stdio.h>
#include <stdint.h>

#include "Network.h"


/// inplementatuon of inet_ntop6 for windows.
/// @return INET6_ADDRSTRLEN sized string
const char *inet_ntop6(
	struct in6_addr *r, ///< [in] in6_addr structure
	char *buf,  ///< [out] buffer where to write resulting string
	socklen_t size  ///< [in] size of in6_addr
);
/// inplementatuon of inet_ntop for windows.
/// @return INET[6]_ADDRSTRLEN sized string
const char * inet_ntop(
	int family, ///< [in] addres famoly - ipv4 or ipv6
	const void *addr, ///< [in] addr structure
	char *buf, ///< [out]  bufer where to write resulting string
	socklen_t size ///< [in] size of addr structure
);



#endif
