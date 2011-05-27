#include "Network.h"

ServerNetwork::ServerNetwork(Server *srvr) {
	this->engine = srvr;
	this->do_ipv4 = true;
	this->do_ipv6 = true;
	
	this->listen_socket_v4 = -1;
	this->listen_socket_v6 = -1;
	FD_ZERO(&this->fd_listen_socks);
	//FD_ZERO(&this->fd_pending_socks);
	//FD_ZERO(&this->fd_accepted_socks);
	this->fd_listen_socks_max = -1;
	//this->fd_pending_socks_max = -1;
	//this->fd_accepted_socks_max = -1;
	memset(this->bind_addres_literal_v4,0,INET_ADDRSTRLEN);
	memset(this->bind_addres_literal_v6,0,INET6_ADDRSTRLEN);
	
	//this->nmsgs = new Messages(msgs_prefix + "/network_server_english.txt");
}

//void ServerNetwork::accept_connection(int remote_sock) {
	//this->connections[remote_sock] = ConnectionAccepted(this->connections_pending[remote_sock]);
//}

void *ServerNetwork::login_server(void *raw_data) {
	ServerNetwork *network_engine = (ServerNetwork *)raw_data;

	char plain_buffer[1024];
	int nbytes;
	struct timeval timeout;

	fd_set read_fds;
	FD_ZERO(&read_fds);
	
	while (true) {
		//read_fds = network_engine->fd_pending_socks;
		//timeout.tv_sec = 0;
		//timeout.tv_usec = 1000 * 1000;
		
		//pn_select(network_engine->fd_pending_socks_max + 1, &read_fds, NULL, NULL, &timeout);
		//for (int i=0; i <= network_engine->fd_pending_socks_max; i++) {

		//}
	}

	pthread_exit(NULL);
	return NULL;
}

//void ServerNetwork::add_pending_connection(int remote_sock,struct sockaddr_storage  remote_addr) {
//	this->connections_pending[remote_sock] = ConnectionPending(remote_addr);
//}

void *ServerNetwork::connection_server(void *raw_data) {
	ServerNetwork *network_engine = (ServerNetwork *)raw_data;	
	
	struct sockaddr_storage remote_addr;
	socklen_t addrlen;
	int remote_sock;

	fd_set read_fds;
	FD_ZERO(&read_fds);
	
	while(true) {
		read_fds = network_engine->fd_listen_socks;
		pn_select(network_engine->fd_listen_socks_max + 1, &read_fds, NULL, NULL, NULL);

		for (int i=0; i <= network_engine->fd_listen_socks_max; i++) {
			if (!FD_ISSET(i,&read_fds)) continue;
			if ((network_engine->do_ipv4 && i == network_engine->listen_socket_v4)
			  ||(network_engine->do_ipv6 && i == network_engine->listen_socket_v6)
			) {
				addrlen = sizeof(remote_addr);
				remote_sock = pn_accept(i,(struct sockaddr *)&remote_addr,&addrlen);
				//FD_SET((unsigned int)remote_sock, &network_engine->fd_pending_socks);
				//if (remote_sock > network_engine->fd_pending_socks_max) {
					//network_engine->fd_pending_socks_max = remote_sock;
				//}
				//network_engine->add_pending_connection(remote_sock,remote_addr);
				//printf("Incoming connection from %s. Pending...\n",network_engine->connections_pending[remote_sock].address_literal);
			}
		}
	}
	
	pthread_exit(NULL);
	return NULL;
}


int ServerNetwork::setup_server_on_addr_port_ipv4(pio_string addr, pio_string port) {
	const char * hostname = addr.c_str();
	if (hostname[0] == '\0') {
		hostname = NULL;
	}
	this->listen_socket_v4 = pnh_bind_to_addr_port(hostname,port.c_str(),AF_INET,SOCK_STREAM,this->bind_addres_literal_v4);
	if (pn_socket_invalid(this->listen_socket_v4)) {
		printf("[ipv4] Failed to bind at %s:%s\n",addr.c_str(),port.c_str());
		return -1;
	}
	printf("[ipv4] Peasantformer server successfuly binded at %s:%s\n",this->bind_addres_literal_v4,port.c_str());
	pn_listen(this->listen_socket_v4,10);
	FD_SET((unsigned int)this->listen_socket_v4, &this->fd_listen_socks);
	if (this->listen_socket_v4 > this->fd_listen_socks_max) {
		this->fd_listen_socks_max = this->listen_socket_v4;
	}
	return 0;
}

int ServerNetwork::setup_server_on_addr_port_ipv6(pio_string addr, pio_string port) {
	
	const char * hostname = addr.c_str();
	if (hostname[0] == '\0') {
		hostname = NULL;
	}
	this->listen_socket_v6 = pnh_bind_to_addr_port(NULL,port.c_str(),AF_INET6,SOCK_STREAM,this->bind_addres_literal_v6);
	
	if (pn_socket_invalid(this->listen_socket_v6)) {
		printf("[ipv6] Failed to bind at %s:%s\n",addr.c_str(),port.c_str());
		return -1;
	}
	printf("[ipv6] Peasantformer server successfuly binded at %s:%s\n",this->bind_addres_literal_v6,port.c_str());
	pn_listen(this->listen_socket_v6,10);
	FD_SET((unsigned int)this->listen_socket_v6, &this->fd_listen_socks);
	if (this->listen_socket_v6 > this->fd_listen_socks_max) {
		this->fd_listen_socks_max = this->listen_socket_v6;
	}
	
	return 0;
}

void ServerNetwork::setup_server_on_addr_port(pio_string addr, pio_string port) {
#ifndef IPV6_V6ONLY
	if (this->do_ipv4) {
		setup_server_on_addr_port_ipv4(addr,port);
	}	
#else
	if (this->do_ipv4 && this->do_ipv6) {
		printf("[ipv4] Your operation system supports dual-IP-stack. Seperate ipv4 socket is not needed.\n");
	} else if (this->do_ipv4) {
		setup_server_on_addr_port_ipv4(addr,port);
	}
#endif

	if (this->do_ipv6) {
		setup_server_on_addr_port_ipv6(addr,port);
	}
		
	pthread_t connection_server_thread;
	pthread_t login_server_thread;
	
	pthread_create(&connection_server_thread,NULL,connection_server,this);
	printf("Connection server fired up\n");
	
	
	pthread_create(&login_server_thread,NULL,login_server,this);
	printf("Login server fired up\n");
	
	//pthread_exit(NULL);
}
