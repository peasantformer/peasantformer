#ifndef PEASANTFORMER_Proto_Network
#define PEASANTFORMER_Proto_Network

/// @file

#ifdef _WIN32

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#endif

#define _WIN32_WINNT 0x501
#include <winsock2.h>
#include <ws2tcpip.h>

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

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#endif	

/// initialize network for current program
/// @return 0, at the moment in all cases
int pn_init();
/// getaddrinfo wrapper
/// @return regular errno code
int pn_getaddrinfo(
	const char *node, ///< [in] node to use
	const char *service, ///< [in] service to use
	const struct addrinfo *hints, ///< [in] hints to use
	struct addrinfo **res ///< [out] result
);
/// freeaddrinfo wrapper
void pn_freeaddrinfo(
	struct addrinfo *res ///< [in,out] ddrinfo destination
);

/// inet_ntop wrapper
/// @return INET[6]_ADDRSTRLEN-sized string representation of address
const char * pn_inet_ntop(
	int af, ///< [in] address family
	const void *src, ///< [in] address to convert
	char *dst, ///< [out] output char buffer
	socklen_t size ///< [in] size of addr strcuture
);
/// socket() wrapper
/// @return regular errno code
int pn_socket(
	int domain, ///< [in] domain
	int type, ///< [in] type
	int protocol ///< [in] protocol
);
/// connect wrapper
/// @return regular errno code
int pn_connect(
	int sockfd, ///< [in] socket connect to
	const struct sockaddr *addr, ///< [in] addr struct
	socklen_t addrlen ///< [in] addr struct length
);
/// listen wrapper
/// @return regular errno code
int pn_listen(
	int sockfd, ///< [in] socket to listen 
	int backlog ///< [in] number of pending connections
);
/// bind wrapper
/// @return regular errno code
int pn_bind(
	int sockfd, ///< [in] socket to bind
	const struct sockaddr *addr, ///< [in] addr struct
	socklen_t addrlen ///< [in] addr struct length
);
/// accept wrapper
/// @return regular errno code
int pn_accept(
	int fd, ///< [in] socket to accept
	struct sockaddr *addr, ///< [in] addr struct
	socklen_t *addrlen ///< [in] addr struct length
);
/// select wrapper
/// @return regular errno code
int pn_select(
	int nfds, ///< [out] maximum of fds + 1
	fd_set *readfds, ///< [in,out] read fds
	fd_set *writefds, ///< [in,out] write fds
	fd_set *exceptfds, ///< [in,out] error fds
	struct timeval *time ///< [in,out] timeout
);
/// checks if socket is valid
/// @return non-zero if true
int pn_socket_invalid(
	int fd ///< [in] socket to check
);
/// setsocketopt wrapper
/// @return regular errno code
int pn_setsockopt(
	int fd, ///< [in] socket fd
	int level, ///< [in] API level
	int optname, ///< [in] option to set
	const void * optval, ///< [in] value for option
	socklen_t len ///< [in] option length
);
/// close fd wrapper
/// @return regular errno code
int pn_close(
	int fd ///< [in] file descriptor to close
);

/// deinitialize network for current program
/// @return 0, at the moment in all cases
int pn_quit();

#endif
