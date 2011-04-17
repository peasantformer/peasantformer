#include <Peasantformer/Network/NetHighLevel.h>


int pnh_set_hints(struct addrinfo *hints, int family, int socktype, int flags) {
	memset(hints,0,sizeof(struct addrinfo));
	hints->ai_family = family;
	hints->ai_socktype = socktype;
	hints->ai_flags = flags;
	return 0;
}
void * pnh_get_in_addr(const struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	} else {
		return &(((struct sockaddr_in6*)sa)->sin6_addr);
	}
}
int pnh_get_addrinfo_literal(const struct addrinfo *res, char *address_literal) {
	if (res->ai_family == AF_INET) {
		pn_inet_ntop(res->ai_family,&((struct sockaddr_in*)res->ai_addr)->sin_addr,address_literal,INET6_ADDRSTRLEN);
	} else {
		pn_inet_ntop(res->ai_family,&((struct sockaddr_in6*)res->ai_addr)->sin6_addr,address_literal,INET6_ADDRSTRLEN);
	}
	return 0;
}
int pnh_get_sockaddr_storage_literal(struct sockaddr_storage *res, char *address_literal) {
	pn_inet_ntop(res->ss_family,pnh_get_in_addr((struct sockaddr*)res),address_literal, INET6_ADDRSTRLEN);
	return 0;
}
int pnh_iterate_addrinfo(struct addrinfo *info, struct addrinfo *res) {
	struct addrinfo *p;
	int sock;

	for (p = info; p != NULL; p = p->ai_next) {
		sock = pn_socket(p->ai_family,p->ai_socktype,p->ai_protocol);
		if (pn_socket_invalid(sock)) continue;
		
		int yes = 1;
		
#ifdef IPV6_V6ONLY
		int no = 0;
		pn_setsockopt(sock,SOL_SOCKET,IPV6_V6ONLY,&no,sizeof(int));
#endif
		pn_setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
		
		if (pn_bind(sock,p->ai_addr,p->ai_addrlen) != 0) {
			pn_close(sock);
			sock = -1;
			continue;
		}
	
		*res = *p;

		return sock;
	}
	return -1;
}
int pnh_bind_to_addr_port(const char *hostname, const char *port, int af, int socktype, struct addrinfo *res, struct addrinfo **localinfo) {
	struct addrinfo hints;
	int sock = -1;
	
	
	pnh_set_hints(&hints,af,socktype,AI_PASSIVE);
	pn_getaddrinfo(hostname, port, &hints, localinfo);
	sock = pnh_iterate_addrinfo(*localinfo,res);
	

	return sock;
}

