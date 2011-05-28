#ifndef PEASANTFORMER_Proto_NetworkHighLevel
#define PEASANTFORMER_Proto_NetworkHighLevel

/// @file

#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "Network.h"

/// hints setup helper function
/// @return non-zero on error
int pnh_set_hints(
	struct addrinfo *hints, ///< [out] hints structure to set
	int family, ///< [in] address family
	int socktype, ///< [in] socket type
	int flags ///< [in] flags
);
/// @return in address
void * pnh_get_in_addr(
	const struct sockaddr *sa ///< [in] sockaddr struct
);
/// het addres literal from addrinfo
/// @return non-zero on error
int pnh_get_addrinfo_literal(
	const struct addrinfo *res, ///< [in] addrinfo struct
	char *address_literal ///< [out] addres literal string buffer
);
/// get sockaddr storage literal
/// @return non-zero on error
int pnh_get_sockaddr_storage_literal(
	struct sockaddr_storage *res, ///< [in] sockaddr_storage struct
	char *address_literal ///< [out] addres literal string buffer
);
/// iterate addrinfo till get some sane addrinfo struct
/// @return non-zero on error
int pnh_iterate_addrinfo(
	struct addrinfo *info, ///< [in] adrinfo struct 
	struct addrinfo *res ///< [out] resulting addrinfo struct
);
/// bind to specified addres and port
/// @return non-zero on error
int pnh_bind_to_addr_port(
	const char *hostname, ///< [in] addres (hostname)
	const char *port, ///< [in] port
	int af, ///< [in] address family
	int socktype, ///< [in] socket type
	char *address_literal ///< [out] addres literal
);
/// socket send string wrapper
/// @return coutn of sent bytes
int pnh_send_str(
	int fd, ///< [in] socket file descriptor 
	const char *buf ///< [in] string to send
);
/// socket send formatted string wrapper
/// @return count of sent bytes
int pnh_send_fstr(
	int fd, ///< [in] socket file descriptor
	size_t bufflen, ///< [in] length of sending buffer
	const char *fmt, ///< [in] format string
	...
);


#endif
