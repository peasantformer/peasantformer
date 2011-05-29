#include "Network.h"

ServerNetwork::ServerNetwork(Server *srvr) {
	this->engine = srvr;
	this->ipv4_socket = -1;
	this->ipv6_socket = -1;
}

ServerNetwork::~ServerNetwork() {
}

int ServerNetwork::setup_server_on_ipv4_address_port(pio_string address,pio_string port) {
	const char *hostname = address.c_str();
	if (hostname[0] == '\0') {
		hostname = NULL;
	}
	this->ipv4_socket = pnh_bind_to_addr_port(hostname,port.c_str(),AF_INET,SOCK_STREAM,this->ipv4_addr_literal);
	if (pn_socket_invalid(this->ipv4_socket)) {
		printf("[ipv4] Failed to bind at %s:%s\n",address.c_str(),port.c_str());
		return -1;
	}
	printf("[ipv4] Peasantformer server successfuly binded at %s:%s\n",this->ipv4_addr_literal,port.c_str());
	pn_listen(this->ipv4_socket,10);
	this->engine->connections->add_listen_socket(this->ipv4_socket);
	return 0;
}

int ServerNetwork::setup_server_on_ipv6_address_port(pio_string address,pio_string port) {
	const char *hostname = address.c_str();
	if (hostname[0] == '\0') {
		hostname = NULL;
	}
	this->ipv6_socket = pnh_bind_to_addr_port(NULL,port.c_str(),AF_INET6,SOCK_STREAM,this->ipv6_addr_literal);
	if (pn_socket_invalid(this->ipv6_socket)) {
		printf("[ipv6] Failed to bind at %s:%s\n",address.c_str(),port.c_str());
		return -1;
	}
	printf("[ipv6] Peasantformer server successfuly binded at %s:%s\n",this->ipv6_addr_literal,port.c_str());
	pn_listen(this->ipv6_socket,10);
	this->engine->connections->add_listen_socket(this->ipv6_socket);
	return 0;
}

void ServerNetwork::setup_server_on_address_port(pio_string address, pio_string port) {
#ifndef IPV6_V6ONLY
	setup_server_on_ipv4_address_port(address,port);
#endif
	setup_server_on_ipv6_address_port(address,port);
}
