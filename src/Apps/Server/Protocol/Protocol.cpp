#include "Protocol.h"

ServerProtocol::ServerProtocol(Server *srvr,pio_string prefix) {
	this->engine = srvr;
	this->nmsgs = new Messages(prefix + "/network_server_english.txt");
}

ServerProtocol::~ServerProtocol() {
	delete this->nmsgs;
}
