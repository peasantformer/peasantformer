#ifndef PEASANTFORMER_Proto_NetworkHighLevel
#define PEASANTFORMER_Proto_NetworkHighLevel

/// @file

#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include "Network.h"

/// Hints setup helper function.
///
/// \param [out] hints is hints structure to set
/// \param [in] family is address family
/// \param [in] socktype is socket type
/// \param [in] flags is hints flags
/// @return non-zero on error
int pnh_set_hints(struct addrinfo *hints, int family, int socktype, int flags);

/// Simple wrapper returning in_addr.
///
/// \param [in] sa is sockaddr struct
/// @return in address
void * pnh_get_in_addr(const struct sockaddr *sa);

/// Get addres literal from addrinfo.
///
/// \param [in] res is addrinfo structure
/// \param [out] address_literal is addres literal string buffer
/// @return non-zero on error
int pnh_get_addrinfo_literal(const struct addrinfo *res, char *address_literal);

/// Get sockaddr storage literal.
///
/// \param [in] res is sockaddr_storage struct
/// \param [out] address_literal is addres_literal string buffer
/// @return non-zero on error
int pnh_get_sockaddr_storage_literal(struct sockaddr_storage *res, char *address_literal);

/// Iterate addrinfo till get some sane addrinfo struct.
///
/// \param [in] info is addrinfo struct
/// \param [out] res is resulting addrinfo struct
/// @return non-zero on error
int pnh_iterate_addrinfo(struct addrinfo *info, struct addrinfo *res);

/// Bind to specified addres and port.
///
/// \param [in] hostname is address hostname to bind on
/// \param [in] port is port to bind on
/// \param [in] af is address family
/// \param [in] socktype is socket type
/// \param [out] address_literal is address literal output buffer
/// @return non-zero on error
int pnh_bind_to_addr_port(const char *hostname, const char *port, int af, int socktype, char *address_literal);

/// Socket send string wrapper.
///
/// \param [in] fd is socket file descriptor 
/// \param [in] buf is string to send
/// @return coutn of sent bytes
int pnh_send_str(int fd, const char *buf);

/// Socket send formatted string wrapper.
///
/// \param [in] fd is socket file descriptor
/// \param [in] bufflen is maximum length of sending string buffer
/// \param [in] fmt is sprintf-format string
/// @return count of sent bytes
int pnh_send_fstr(int fd, size_t bufflen, const char *fmt, ...);


#endif
