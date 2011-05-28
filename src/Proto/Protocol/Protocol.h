#ifndef PEASANTFORMER_Proto_Protocol
#define PEASANTFORMER_Proto_Protocol

#include <Proto/PlainIO/PlainIOHighLevel.h>
#include <Proto/Network/NetworkHighLevel.h>
#include <Proto/Messages/Messages.h>
#include <Data/CircularBuffer/CircularBuffer.h>

class Protocol {
	private:
		int buffsize;
		Messages *msgs;
		CircularBuffer<wchar_t> *circus;
	public:
		Protocol(int buffsize, Messages *msgs);
		~Protocol();
	public:
};

#endif
