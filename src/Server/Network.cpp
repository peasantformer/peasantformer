#include "Server.h"

void *ServerEngine::game_server(void *data) {
	ServerEngine *engine = (ServerEngine *)data;
	while (true) {

	}
	pthread_exit(NULL);
	return NULL;
}
void *ServerEngine::login_server(void *data) {
	ServerEngine *engine = (ServerEngine *)data;

	char plain_buffer[1024];
	int nbytes;
	struct timeval timeout;

	fd_set read_fds;
	FD_ZERO(&read_fds);

	
	while (true) {
		read_fds = engine->pending_socks;
		timeout.tv_sec = 0;
		timeout.tv_usec = 1000 * 1000;

		pn_select(engine->fd_pending_max + 1, &read_fds, NULL, NULL, &timeout);
		for (int i=0; i <= engine->fd_pending_max; i++) {
			if (!FD_ISSET(i,&engine->pending_socks)) continue;

			engine->pending_connections[i]->iterations++;
			if (engine->pending_connections[i]->iterations > 420) {
				pn_close(i);
				FD_CLR((unsigned int)i,&engine->pending_socks);
				printf("Pending connection %s timed out.\n",engine->pending_connections[i]->address_literal);
				delete engine->pending_connections[i];
			}

			if (!FD_ISSET(i,&read_fds)) continue;
			if ((nbytes = recv(i,plain_buffer,sizeof(plain_buffer)-1,0)) <= 0) {
				pn_close(i);
				FD_CLR((unsigned int)i,&engine->pending_socks);
				printf("Pending connection %s closed by client.\n",engine->pending_connections[i]->address_literal);
				delete engine->pending_connections[i];
			} else {
				plain_buffer[nbytes] = '\0';
				wchar_t plain_name[33] = {0};
				wchar_t plain_password_hash[33] = {0};
//				for (int z=0; z < nbytes; z++) {
//					printf("%d ",plain_buffer[z]);
//				}
				pio_string pstr = pio_string(plain_buffer);
				swscanf(pstr.w_str(),L"LOGIN %32lc %32lc",plain_password_hash,plain_name);

				pio_string name(plain_name);
				pio_string password_hash(plain_password_hash);

				if (password_hash.length() != 32) {
					FD_CLR((unsigned int)i,&engine->pending_socks);
					printf("Pending connection %s closed due to invalid auth.\n",engine->pending_connections[i]->address_literal);
					delete engine->pending_connections[i];

					pnh_send_str(i,engine->nmsgs->get("password_hash_too_short"));
	///				send(i,close_message,sizeof(close_message),0);
					pn_close(i);
					continue;
				}
				
				name.weedout_control();
				password_hash.weedout_control();

				pio_string got_username;
				pio_string got_nickname;
				pio_string got_hash;
				
				int ret;

				ret = engine->db->login->lookup_user_by_password_username(
					name,
					password_hash,
					got_username,
					got_nickname,
					got_hash
				);
				
				if (ret < 0) {
					FD_CLR((unsigned int)i,&engine->pending_socks);
					printf("Pending connection %s closed due to failed auth.\n",engine->pending_connections[i]->address_literal);
					delete engine->pending_connections[i];

					pnh_send_str(i,engine->nmsgs->get("wrong_auth"));
					pn_close(i);
					continue;
				}

				printf("%s - %s\n",got_username.c_str(),got_hash.c_str());
			}

		}

	}


	pthread_exit(NULL);
	return NULL;
}
void *ServerEngine::connection_server(void *data) {
	ServerEngine *engine = (ServerEngine *)data;
	
	struct sockaddr_storage remote_addr;
	socklen_t addrlen;
	int remote_sock;

	fd_set read_fds;
	FD_ZERO(&read_fds);

	while(true) {
		read_fds = engine->listen_master;
		pn_select(engine->fd_master_max + 1, &read_fds, NULL, NULL, NULL);
		for (int i=0; i <= engine->fd_master_max; i++) {
			if (!FD_ISSET(i,&read_fds)) continue;
			if ((engine->do_ipv4 && i == engine->listen_socket_v4)
			  ||(engine->do_ipv6 && i == engine->listen_socket_v6)
			) {
				addrlen = sizeof(remote_addr);
				remote_sock = pn_accept(i,(struct sockaddr *)&remote_addr,&addrlen);
				FD_SET((unsigned int)remote_sock, &engine->pending_socks);
				if (remote_sock > engine->fd_pending_max) {
					engine->fd_pending_max = remote_sock;
				}
				engine->pending_connections[remote_sock] = new PendingConnection(remote_addr);
				printf("Incoming connection from %s. Pending...\n",engine->pending_connections[remote_sock]->address_literal);
			}
		}
	}
	
	pthread_exit(NULL);
	return NULL;
}


/*
void *ServerEngine::game_server(void *data) {
	ServerEngine *engine = (ServerEngine *)data;
	char plain_buffer[1024];
	int nbytes;
	fd_set read_fds;

	FD_ZERO(&read_fds);

	struct timeval timeout;


	while (true) {
		timeout.tv_sec = 0;
		timeout.tv_usec = 1 * 1000;
		read_fds = engine->accepted_socks;
		pn_select(engine->fd_max + 1, &read_fds, NULL, NULL, &timeout);
		for (int i=0; i <= engine->fd_max; i++) {
			if (!FD_ISSET(i,&read_fds)) continue;
			if ((nbytes = recv(i,plain_buffer,sizeof(plain_buffer),0)) <= 0) {
				printf("Recv %d\n",nbytes);
				if (nbytes == 0) {
					printf("Connection from %s closed\n",engine->connections[i]->address_literal);
				}
				pn_close(i);
				FD_CLR((unsigned int)i,&engine->accepted_socks);
			} else {
				if (!engine->connections[i]->isLogged) continue;
				plain_buffer[nbytes] = '\0';
				printf("%s\n",plain_buffer);
			}
		}

	}

	pthread_exit(NULL); 
}

void *ServerEngine::login_server(void *data) {

}

void *ServerEngine::connection_server(void *data) {
	ServerEngine *engine = (ServerEngine *)data;
	fd_set master_read_fds;
	struct sockaddr_storage remote_addr;
	socklen_t addrlen;
	int remote_sock;

	char plain_buffer[1024];
	int nbytes;

	FD_ZERO(&master_read_fds);
	
	while (true) { // do 
		master_read_fds = engine->listen_master;
		pn_select(engine->fd_master_max + 1, &master_read_fds, NULL, NULL, NULL);
		for (int i=0; i <= engine->fd_master_max; i++) {
			if (!FD_ISSET(i,&master_read_fds)) continue;
			if (  (engine->do_ipv4 && i == engine->listen_socket_v4)
				||(engine->do_ipv6 && i == engine->listen_socket_v6)
			) {
				addrlen = sizeof(remote_addr);
				remote_sock = pn_accept(i,(struct sockaddr *)&remote_addr,&addrlen);
				FD_SET((unsigned int)remote_sock, &engine->accepted_socks);
				if (remote_sock > engine->fd_max) {
					engine->fd_max = remote_sock;
				}
				engine->connections[remote_sock] = new IncomingConnection(remote_addr);
				printf("Incoming connection from %s\n",engine->connections[remote_sock]->address_literal);
			}
		}
	}

	pthread_exit(NULL); // causes memleak, but it is ok because this memleak is
	                    // shared between all pthread_exit( ) calls and results
						// only in single 56-bytes block for whole program.
	return NULL;
}

*/

int ServerEngine::setup_server_on_addr_port_ipv4(const char *hostname, const char *raw_hostname, const char *port) {
	this->listen_socket_v4 = pnh_bind_to_addr_port(hostname,port,AF_INET,SOCK_STREAM,this->bind_addres_literal_v4);
	if (pn_socket_invalid(this->listen_socket_v4)) {
		printf("[ipv4] Failed to bind at %s:%s\n",raw_hostname,port);
		return -1;
	}
	printf("[ipv4] Peasantformer server successfuly binded at %s:%s\n",this->bind_addres_literal_v4,port);
	pn_listen(this->listen_socket_v4,10);
	FD_SET((unsigned int)this->listen_socket_v4, &this->listen_master);
	if (this->listen_socket_v4 > this->fd_master_max) {
		this->fd_master_max = this->listen_socket_v4;
	}
	return 0;
}

int ServerEngine::setup_server_on_addr_port_ipv6(const char *hostname, const char *raw_hostname, const char *port) {
	this->listen_socket_v6 = pnh_bind_to_addr_port(hostname,port,AF_INET6,SOCK_STREAM,this->bind_addres_literal_v6);
	if (pn_socket_invalid(this->listen_socket_v6)) {
		printf("[ipv6] Failed to bind at %s:%s\n",raw_hostname,port);
		return -1;
	}
	printf("[ipv6] Peasantformer server successfuly binded at %s:%s\n",this->bind_addres_literal_v6,port);
	pn_listen(this->listen_socket_v6,10);
	FD_SET((unsigned int)this->listen_socket_v6, &this->listen_master);
	if (this->listen_socket_v6 > this->fd_master_max) {
		this->fd_master_max = this->listen_socket_v6;
	}
	return 0;
}

int ServerEngine::setup_server_on_addr_port(const char *hostname, const char *port) {
	const char *raw_hostname = hostname;
	
	if (hostname[0] == '\0') {
		hostname = NULL;
	}
#ifndef IPV6_V6ONLY
	if (this->do_ipv4) {
		setup_server_on_addr_port_ipv4(hostname,raw_hostname,port);
	}
#else
	if (this->do_ipv4 && this->do_ipv6) {
		printf("[ipv4] Your operation system supports dual-IP-stack. Seperate ipv4 socket is not needed.\n");
	} else if (this->do_ipv4) {
		setup_server_on_addr_port_ipv4(hostname,raw_hostname,port);
	}
#endif

	if (this->do_ipv6) {
		setup_server_on_addr_port_ipv6(hostname,raw_hostname,port);
	}
	
	pthread_t connection_server_thread;
	pthread_t login_server_thread;
	pthread_t game_server_thread;
	
	pthread_create(&connection_server_thread,NULL,connection_server,this);
	printf("Connection server fired up\n");
	
	pthread_create(&login_server_thread,NULL,login_server,this);
	printf("Login server fired up\n");

	pthread_create(&game_server_thread,NULL,game_server,this);
	printf("Game server fired up\n");

	pthread_join(game_server_thread,NULL);

	pthread_exit(NULL);
	return 0;
}
