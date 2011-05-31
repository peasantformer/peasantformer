#include "ServerNetwork.h"

/// @file

int ServerNetwork::setup_ipv4_listener(PString address, PString port) {
	const char *hostname = address.c_str();
	if (hostname[0] == '\0') {
		hostname = NULL;
	}
	ipv4_listen_socket = pnh_bind_to_addr_port(hostname,port.c_str(),AF_INET,SOCK_STREAM,bind_address_ipv4_literal);
	if (pn_socket_invalid(ipv4_listen_socket)) {
		printf("[ipv4] Failed to bind on %s:%s\n",address.c_str(),port.c_str());
		return -1;
	}
	printf("[ipv4] Successfult bind on %s:%s\n",bind_address_ipv4_literal,port.c_str());
	pn_listen(ipv4_listen_socket,10);
	return 0;
}

int ServerNetwork::setup_ipv6_listener(PString address, PString port) {
	return 0;
}

ServerNetwork::ServerNetwork(Server *srvr) {
	engine = srvr;
	
	ipv4_listen_socket = -1;
	ipv6_listen_socket = -1;
}

ServerNetwork::~ServerNetwork() {
}

void ServerNetwork::setup(PString address, PString port) {
}

int ServerNetwork::get_ipv4_socket() {
	return ipv4_listen_socket;
}

int ServerNetwork::get_ipv6_socket() {
	return ipv6_listen_socket;
}
