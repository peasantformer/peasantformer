#include "Server.h"

void *ServerEngine::login_server(void *data) {
	ServerEngine *engine = (ServerEngine *)data;
	char plain_buffer[1024];
	fd_set read_fds;
	struct sockaddr_storage remote_addr;
	socklen_t addrlen;
	int nbytes;
	int remote_sock;
		
	FD_ZERO(&read_fds);
	
	while (true) {
		read_fds = engine->listen_master;
		pn_select(engine->fd_max + 1, &read_fds,NULL,NULL,NULL);
		for (int i=0; i <= engine->fd_max; i++) {
			if (!FD_ISSET(i,&read_fds)) continue;
			if ((engine->do_ipv4 && i == engine->listen_socket_v4) || (engine->do_ipv6 && i == engine->listen_socket_v6)) {
				// handle new connection
				addrlen = sizeof(remote_addr);
				remote_sock = pn_accept(i,(struct sockaddr *)&remote_addr,&addrlen);
				FD_SET(remote_sock, &engine->listen_master);
				if (remote_sock > engine->fd_max) {
					engine->fd_max = remote_sock;
				}
				engine->connections[remote_sock] = IncomingConnection(remote_addr);
				printf("New connection from %s\n",engine->connections[remote_sock].address_literal);
			} else {
				// handle existing connection
				if ((nbytes = recv(i,plain_buffer,sizeof(plain_buffer),0)) <= 0) {
					printf("Recv %d\n",nbytes);
					if (nbytes == 0) {
						printf("Connection from %s closed\n",engine->connections[i].address_literal);
					}
					pn_close(i);
					FD_CLR(i,&engine->listen_master);
				} else {
					// handle remote input
					
				}
			}
		}
	}

	return NULL;
}

int ServerEngine::setup_server_on_addr_port(const char *hostname, const char *port) {
	const char *raw_hostname = hostname;
	
	if (hostname[0] == '\0') {
		hostname = NULL;
	}
#ifndef IPV6_V6ONLY
	if (this->do_ipv4) {
		this->listen_socket_v4 = pnh_bind_to_addr_port(hostname,port,AF_INET,SOCK_STREAM,this->bind_addres_literal_v4);
		if (pn_socket_invalid(this->listen_socket_v4)) {
			printf("[ipv4] Failed to bind at %s:%s\n",raw_hostname,port);
			return -1;
		}
		printf("[ipv4] Peasantformer server successfuly binded at %s:%s\n",this->bind_addres_literal_v4,port);
		pn_listen(this->listen_socket_v4,10);
		FD_SET(this->listen_socket_v4, &this->listen_master);
		if (this->listen_socket_v4 > this->fd_max) {
			this->fd_max = this->listen_socket_v4;
		}
	}
#else
	if (this->do_ipv4) {
		printf("[ipv4] Your operation system supports dual-IP-stack. Seperate ipv4 socket is not needed.\n");
	}
#endif

	if (this->do_ipv6) {
		this->listen_socket_v6 = pnh_bind_to_addr_port(hostname,port,AF_INET6,SOCK_STREAM,this->bind_addres_literal_v6);
		if (pn_socket_invalid(this->listen_socket_v6)) {
			printf("[ipv6] Failed to bind at %s:%s\n",raw_hostname,port);
			return -1;
		}
		printf("[ipv6] Peasantformer server successfuly binded at %s:%s\n",this->bind_addres_literal_v6,port);
		pn_listen(this->listen_socket_v6,10);
		FD_SET(this->listen_socket_v6, &this->listen_master);
		if (this->listen_socket_v6 > this->fd_max) {
			this->fd_max = this->listen_socket_v6;
		}
	}
	
	pthread_t login_server_thread;
	
	pthread_create(&login_server_thread,NULL,login_server,this);

	pthread_join(login_server_thread,NULL);
	
	return 0;
}
