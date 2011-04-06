#include "Client.h"

int main(int argc, char **argv) {
	int status = 0;
	struct addrinfo hints;
	struct addrinfo *servinfo;
	struct addrinfo *res;
	char address[INET6_ADDRSTRLEN];
	int sock;
	
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	
	status = getaddrinfo(argv[1],"80",&hints,&servinfo);
	
	if (status != 0) {
		printf("getaddrinfo() failed with error %d\n",status);
		return -1;
	}
	
	for (struct addrinfo *p = servinfo; p != NULL; p = p->ai_next) {
		if (p->ai_family == AF_INET) {
			inet_ntop(p->ai_family,&((struct sockaddr_in*)p->ai_addr)->sin_addr,address,INET6_ADDRSTRLEN);
			printf("IPV4 ");
		} else {
			inet_ntop(p->ai_family,&((struct sockaddr_in6*)p->ai_addr)->sin6_addr,address,INET6_ADDRSTRLEN);
			printf("IPV6 ");
		}
		printf("%s\n",address);
		res = p;
	//	break; // suppose the first value is a valid one;
	}
	
	
	sock = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	
	if (sock < 0) {
		printf("socket() failed with result %d\n",sock);
		return -1;
	}
	
	
	status = connect(sock, res->ai_addr, res->ai_addrlen);
	
	if (status < 0) {
		printf("connect() failed with error %d\n",status);
		return -1;
	}
	
	
	return 0;
}
