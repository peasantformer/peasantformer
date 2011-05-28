#include "Protocol.h"

Protocol::Protocol(int buffsize, Messages *msgs) {
	this->msgs = msgs;
	this->buffsize = buffsize;	
	this->circus = new CircularBuffer<wchar_t>(buffsize);
}

Protocol::~Protocol() {
	delete this->circus;
}
