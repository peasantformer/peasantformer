#include <Peasantformer/Server/Server.h>

void *ServerEngine::listener(void *raw_data) {
	ServerEngine *engine = (ServerEngine *)raw_data;
	
	char address_literal[INET6_ADDRSTRLEN];
	char plain_buffer[1024];
	fd_set read_fds;
	struct timeval tv;
	struct sockaddr_storage remote_addr;

	socklen_t addrlen;
	int nbytes;
	int newsock;
	
	FD_ZERO(&read_fds);
	

	while (true) {
		read_fds = engine->master;
		//tv.tv_sec = 0;
		//tv.tv_usec = 500000;
		pn_select(engine->fd_max + 1, &read_fds,NULL,NULL,NULL);
		printf("select update\n");
		for (int i=0; i <= engine->fd_max; i++) {
			if (FD_ISSET(i, &read_fds)) {
				if ((engine->ipv4 && i == engine->v4_socket) || (engine->ipv6 && i == engine->v6_socket)) {
					addrlen = sizeof(remote_addr);
					newsock = pn_accept(i,(struct sockaddr*)&remote_addr,&addrlen);
					FD_SET(newsock,&engine->master);
					if (newsock > engine->fd_max) {
						engine->fd_max = newsock;
					}
					engine->connections[newsock]=ServerConnection(remote_addr);
					pnh_get_sockaddr_storage_literal(&remote_addr,address_literal);
					printf("New connection from %s\n",address_literal);
				} else {
					if ((nbytes = recv(i,plain_buffer,sizeof(plain_buffer),0)) <= 0) {
						if (nbytes == 0) {
							printf("Connection from %s closed\n",engine->connections[i].address_literal);
						}
						pn_close(i);
						FD_CLR(i,&engine->master);
					} else { /* we got some data */
						engine->buffer->write(plain_buffer,nbytes);
					}
				}
			}
		}
	}
	pthread_exit(NULL);
}

int ServerEngine::setup_server_on_addr_port(const char *hostname, const char *port, bool ipv4, bool ipv6) {
	struct addrinfo **localinfo = (struct addrinfo **)malloc(1 * sizeof (struct addrinfo *));
	localinfo[0] = NULL;
	struct addrinfo resv4;
	struct addrinfo resv6;
	char address_literal[INET6_ADDRSTRLEN];
	const char *hostname_literal = hostname;
	int newsock = -1;
	this->fd_max = -1;


	
	FD_ZERO(&master);

	
	if (hostname[0] == '\0') {
		hostname = NULL;
	}

	this->v4_socket = -1;
	this->v6_socket = -1;

#ifndef IPV6_V6ONLY
	if (ipv4) {
		this->v4_socket = pnh_bind_to_addr_port(NULL, port,AF_INET,SOCK_STREAM,&resv4,localinfo);
		if (pn_socket_invalid(this->v4_socket)) {
			printf("[ipv4] Failed to bind at %s:%s\n",hostname_literal,port);
			return -1;
		}
		pnh_get_addrinfo_literal(&resv4,address_literal);
		printf("[ipv4] Peasantformer server successfuly bined to %s:%s\n",address_literal,port);
		pn_freeaddrinfo(*localinfo);
		pn_listen(this->v4_socket,10);
		FD_SET(this->v4_socket, &master);
		if (this->v4_socket > fd_max) {
			this->fd_max = this->v4_socket;
		}
	}
#else
	if (ipv4) {
		printf("[ipv4] Your system supports dual-IP stack. Seperate ipv4 socket is not needed.\n");
	}

#endif

	if (ipv6) {
		this->v6_socket = pnh_bind_to_addr_port(NULL, port,AF_INET6,SOCK_STREAM,&resv6,localinfo);
		if (pn_socket_invalid(this->v6_socket)) {
			printf("[ipv6] Failed to bind at %s:%s\n",hostname_literal,port);
			return -1;
		}
		pnh_get_addrinfo_literal(&resv6,address_literal);
		printf("[ipv6] Peasantformer server successfuly bined to %s:%s\n",address_literal,port);
		pn_freeaddrinfo(*localinfo);
		pn_listen(this->v6_socket,10);
		FD_SET(this->v6_socket, &master);
		if (this->v6_socket > fd_max) {
			this->fd_max = this->v6_socket;
		}
	}
	
	pthread_t networking_thread;
	
	pthread_create(&networking_thread,NULL,listener,this);
	
	pthread_join(networking_thread,NULL);
	//pthread_kill(networking_thread,10);
	//listener(master, fd_max, ipv4, ipv6);
	printf("workin\n");
	
	free(localinfo);
	pthread_exit(NULL);
	return 0;
}
