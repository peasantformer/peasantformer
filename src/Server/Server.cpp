#include "Server.h"


int set_hints(struct addrinfo *hints, int family, int socktype, int flags) {
	memset(hints,0,sizeof(struct addrinfo));
	hints->ai_family = family;
	hints->ai_socktype = socktype;
	hints->ai_flags = flags;
	return 0;
}

void *get_in_addr(struct sockaddr *sa) {
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	} else {
		return &(((struct sockaddr_in6*)sa)->sin6_addr);
	}	
	return NULL;
}

int iterate_addrinfo(struct addrinfo *remote_info, struct addrinfo *res, int *out_sock, char *address_literal) {
	struct addrinfo *p;
	
	for (p = remote_info; p != NULL; p = p->ai_next) {
		if ((*out_sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0) {
			continue;
		}
				
		int yes = 1;
		setsockopt(*out_sock,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
		if (bind(*out_sock,p->ai_addr,p->ai_addrlen) < 0) {
			continue;
		}
		
		res = p;
		
		if (p->ai_family == AF_INET) {
			inet_ntop(p->ai_family,&((struct sockaddr_in*)p->ai_addr)->sin_addr,address_literal,INET6_ADDRSTRLEN);
		} else {
			inet_ntop(p->ai_family,&((struct sockaddr_in6*)p->ai_addr)->sin6_addr,address_literal,INET6_ADDRSTRLEN);
		}
		
		return 0;
	}
	
	return -1;
}

int make_bind(const char *hostname, const char *port, int type, struct addrinfo *res, int *sock_listen, char *address_literal) {
	int status = -1;
	struct addrinfo hints, *remote_info;
	
	
	*sock_listen = -1;
	
	status = set_hints(&hints,AF_INET6,type,AI_PASSIVE);
	if (status == 0) {
		status = getaddrinfo(hostname,port,&hints,&remote_info);
		if (status == 0) {
			status = iterate_addrinfo(remote_info, res, sock_listen, address_literal);
			
		}
	}

	if (status < 0) {
		
		status = set_hints(&hints,AF_INET,type,AI_PASSIVE);
		if (status == 0) {
			status = getaddrinfo(hostname,port,&hints,&remote_info);
			if (status == 0) {
				status = iterate_addrinfo(remote_info, res, sock_listen, address_literal);
				
			}
		}
	}
	
	
	if (status < 0) return -1;
	
	return 0;
}


int main(int argc, char **argv) {
	
	std::string port("50600");
	
	char address_literal[INET6_ADDRSTRLEN];
	struct addrinfo res;
	int sock_listen, sock_remote;
	struct sockaddr_storage remote_addr;
	socklen_t addr_size = sizeof(struct sockaddr_storage);
	
	
	if (make_bind(NULL, port.c_str(), SOCK_DGRAM, &res, &sock_listen,address_literal) < 0) {
		printf("Failed to bind\n");
		return -1;
	}

	printf("Successfuly binded on %s\n",address_literal);
	
//	listen(sock_listen,10);
	char buf[4] = {0};
	//sendto(sock_listen,"abc",4,0,res->ai_addr,res->ai_addrlen);
	while (1) {
		//sock_remote = accept(sock_listen,(struct sockaddr*)&remote_addr, &addr_size);
		//inet_ntop(remote_addr.ss_family,get_in_addr((struct sockaddr*)&remote_addr),address_literal,INET6_ADDRSTRLEN);
		//printf("Connection from %s\n",address_literal);
		//send(sock_remote,"Oh hai!~~~~~~~~~~~~",13,0);
		recvfrom(sock_listen,buf,3,0,(struct sockaddr*)&remote_addr,&addr_size);
		sendto(sock_listen,"abc",4,0,(struct sockaddr*)&remote_addr,addr_size);
		 printf("listener: got packet from %s\n",buf);
		//sendto(sock_listen,"abc",4,0,res.ai_addr,res.ai_addrlen);
		//sendto
		//CLOSE_SOCKET(sock_remote);
	}
	
	CLOSE_SOCKET(sock_listen);
	return 0;
}
