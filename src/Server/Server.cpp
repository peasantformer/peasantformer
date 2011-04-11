#include "Server.h"
/*

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

int get_addrinfo_literal(struct addrinfo *res, char *address_literal) {
	if (res->ai_family == AF_INET) {
		inet_ntop(res->ai_family,&((struct sockaddr_in*)res->ai_addr)->sin_addr,address_literal,INET6_ADDRSTRLEN);
	} else {
		inet_ntop(res->ai_family,&((struct sockaddr_in6*)res->ai_addr)->sin6_addr,address_literal,INET6_ADDRSTRLEN);
	}
	return 0;
}


int get_sockaddr_storage_literal(struct sockaddr_storage *res, char *address_literal) {
	inet_ntop(res->ss_family,get_in_addr((struct sockaddr*)res),address_literal, INET6_ADDRSTRLEN);
	return 0;
}

int iterate_addrinfo(struct addrinfo *info, struct addrinfo *res) {
	struct addrinfo *p;
	
	int sock;
	
	for (p = info; p != NULL; p = p->ai_next) {
		sock = socket(p->ai_family,p->ai_socktype,p->ai_protocol);
		if (sock < 0) continue;
		
		int yes = 1;
		setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int));
		
		if (bind(sock,p->ai_addr,p->ai_addrlen) < 0) {
			CLOSE_SOCKET(sock);
			sock = -1;
			continue;
		}
		
		*res = *p;
		return sock;
	}
	return -1;
}

int bind_to_addr_port(char *hostname, char *port, int socktype, struct addrinfo *res) {
	struct addrinfo *localinfo;
	struct addrinfo hints;
	
	int sock = -1;
	
	set_hints(&hints,AF_INET6,socktype,AI_PASSIVE);
	getaddrinfo(hostname,port,&hints,&localinfo);
	sock = iterate_addrinfo(localinfo,res);

	freeaddrinfo(localinfo);
	if (sock >= 0) return sock;
	
	set_hints(&hints,AF_INET,socktype,AI_PASSIVE);
	getaddrinfo(hostname,port,&hints,&localinfo);
	sock = iterate_addrinfo(localinfo,res);
	
	
	freeaddrinfo(localinfo);
	return sock;
}
*/
int main(int argc, char **argv) {
	/*
	char *hostname = NULL;
	char port[] = "50600";
	
	int listener_socket;
	int remote_socket;
	struct addrinfo res;
	struct sockaddr_storage remoteinfo;
	socklen_t addrlen;
	
	fd_set master;
	fd_set read_fds;
	int fdmax;
	
	char buf[4];
	int nbytes;
	
	char address_literal[INET6_ADDRSTRLEN];
	
	FD_ZERO(&master);
	FD_ZERO(&read_fds);
	
	listener_socket = bind_to_addr_port(hostname,port,SOCK_STREAM,&res);
	
	if (listener_socket < 0) {
		printf("Failed to bind\n");
		return -1;
	}
	
	get_addrinfo_literal(&res,address_literal);
	printf("Successfuly binded to %s\n",address_literal);
	*/
	/*
	listen(listener_socket,10);
	
	FD_SET(listener_socket,&master);
	
	fdmax = listener_socket;
	
	while (true) {
		read_fds = master;
		if (select(fdmax+1,&read_fds,NULL,NULL,NULL) < 0) {
			printf("Select error!");
			return -1;
		}
		//printf("Still alive\n");
		for (int i=0; i <= fdmax; i++) {
			if (FD_ISSET(i,&read_fds)) {
				if (i == listener_socket) {
					addrlen = sizeof(remoteinfo);
					remote_socket = accept(listener_socket,(struct sockaddr*)&remoteinfo,&addrlen);
					get_sockaddr_storage_literal(&remoteinfo,address_literal);
					printf("Got connection from %s\n",address_literal);
					FD_SET(remote_socket,&master);
					if (remote_socket > fdmax) {
						fdmax = remote_socket;
					}
				} else {
					nbytes = recv(i,buf,sizeof(buf) - 1,0);
					buf[nbytes] = '\0';
					if (nbytes <= 0) {
						printf("Hung up on %d\n",i);
						CLOSE_SOCKET(i);
						FD_CLR(i,&master);
					} else {
						printf("%s",buf);
						for (int n=0; n <= fdmax; n++) {
							if(n == i || n == listener_socket) continue;
							if (!FD_SET(n,&master)) continue;
							send(n,buf,nbytes,0);
						}
					}
					
					
					
				}
			}
		}
	}
	
	*/
	
	/*
	std::string port("50600");
	
	char address_literal[INET6_ADDRSTRLEN];
	struct addrinfo res;
	int sock_listen, sock_remote;
	struct sockaddr_storage remote_addr;
	socklen_t addr_size = sizeof(struct sockaddr_storage);
	
	
	if (make_bind(NULL, port.c_str(), SOCK_STREAM, &res, &sock_listen,address_literal) < 0) {
		printf("Failed to bind\n");
		return -1;
	}

	printf("Successfuly binded on %s\n",address_literal);
	
	
	//char buf[4] = {0};
	//sendto(sock_listen,"abc",4,0,res->ai_addr,res->ai_addrlen);
	//while (1) {
		sock_remote = accept(sock_listen,(struct sockaddr*)&remote_addr, &addr_size);
		inet_ntop(remote_addr.ss_family,get_in_addr((struct sockaddr*)&remote_addr),address_literal,INET6_ADDRSTRLEN);
		printf("Connection from %s\n",address_literal);
		//send(sock_remote,"Oh hai!~~~~~~~~~~~~",13,0);
		//recvfrom(sock_listen,buf,3,0,(struct sockaddr*)&remote_addr,&addr_size);
		//sendto(sock_listen,"abc",4,0,(struct sockaddr*)&remote_addr,addr_size);
		//printf("listener: got packet from %s\n",buf);
		//sendto(sock_listen,"abc",4,0,res.ai_addr,res.ai_addrlen);
		//sendto
		//CLOSE_SOCKET(sock_remote);
	//}
	
	CLOSE_SOCKET(sock_listen);
	*/
	return 0;
}
