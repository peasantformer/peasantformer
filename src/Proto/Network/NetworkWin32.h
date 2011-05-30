#ifndef PEASANTFORMER_Proto_NetworkWin32
#define PEASANTFORMER_Proto_NetworkWin32

/// @file

#include <stdio.h>
#include <stdint.h>

#include "Network.h"


/// Implementatuon of inet_ntop6 for windows.
///
/// \param [in] r is in6_addr structure
/// \param [out] buf is buffer where result will be written to
/// \param [in] size is size of in6_addr structure
/// @return INET6_ADDRSTRLEN sized string
const char *inet_ntop6(struct in6_addr *r, char *buf, socklen_t size);

/// Implementatuon of inet_ntop for windows.
///
/// \param [in] family is ip addres family - ipv4 or ipv6
/// \param [in] addr is addr structure
/// \param [out] buf is buffer where reuslt will be written to
/// \param [in] size is size of addr structure
/// @return INET[6]_ADDRSTRLEN sized string
const char * inet_ntop(int family, const void *addr, char *buf, socklen_t size);



#endif
