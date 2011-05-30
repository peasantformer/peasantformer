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

#include <Proto/Network/NetworkWin32.h>

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#endif	

/// Initialize network for current program.
///
/// @return 0, at the moment in all cases
int pn_init();

/// Getaddrinfo() wrapper.
///
/// \param [in] node to use
/// \param [in] service to use
/// \param [in] hints to use
/// \param [out] res is resulting addrinfo struct pointer
/// @return regular errno code
int pn_getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);

/// Freeaddrinfo() wrapper.
///
/// \param [in,out] res is addrinfo destination
void pn_freeaddrinfo(struct addrinfo *res);

/// Inet_ntop() wrapper.
///
/// \param [in] af is address family
/// \param [in] src is address to convert
/// \param [out] dst destination buffer
/// \param [in] size is size of af structure 
/// @return INET[6]_ADDRSTRLEN-sized string representation of address
const char * pn_inet_ntop(int af, const void *src, char *dst, socklen_t size);

/// Socket() wrapper.
///
/// \param [in] domain to use
/// \param [in] type of socket
/// \param [in] protocol to use
/// @return regular errno code
int pn_socket(int domain, int type, int protocol);

/// Connect() wrapper.
//
/// \param [in] sockfd is socket connect to
/// \param [in] addr is addr struct
/// \param [in] addrlen is addr struct length
/// @return regular errno code
int pn_connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

/// Listen() wrapper
///
/// \param [in] sockfd is socket file descriptor to listen
/// \param [in] backlog is number of pending connections
/// @return regular errno code
int pn_listen(int sockfd,int backlog);

/// Bind() wrapper.
///
/// \param [in] sockfd is socket file descriptor to bind on
/// \param [in] addr struct to use
/// \param [in] addrlen is length of addr struct
/// @return regular errno code
int pn_bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

/// Accept() wrapper.
///
/// \param [in] fd is socket to accept
/// \param [in] addr is addr struct
/// \param [in] addrlen is addr struct length
/// @return regular errno code
int pn_accept(int fd, struct sockaddr *addr, socklen_t *addrlen);

/// Select() wrapper.
///
/// \param [in] nfds 	 maximum of fds + 1
/// \param [in,out] readfds is fd_set of reading sockets
/// \param [in,out] writefds is fd_set of writing sockets
/// \param [in,out] exceptfds is fd_set of exceptioned sockets
/// \param [in,out] time is timeout for select()
/// @return regular errno code
int pn_select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,struct timeval *time);

/// Checks if socket is valid.
///
/// \param [in] fd is socket file descriptor to check
/// @return non-zero if true
int pn_socket_invalid(int fd);

/// Setsocketopt() wrapper.
///
/// \param [in] fd is socket fd
/// \param [in] level is API level
/// \param [in] optname is option to set
/// \param [in] optval is value to set
/// \param [in] len is length of option value
/// @return regular errno code
int pn_setsockopt(int fd, int level, int optname, const void * optval, socklen_t len);

/// Close() fd wrapper.
///
/// \param [in] fd is fd to close
/// @return regular errno code
int pn_close(int fd);

/// Deinitialize network for current program.
///
/// @return 0, at the moment in all cases
int pn_quit();

#endif
