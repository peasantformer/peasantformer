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
	
	status = pn_getaddrinfo(argv[1],"50600",&hints,&servinfo);
	
	return 0;
}
