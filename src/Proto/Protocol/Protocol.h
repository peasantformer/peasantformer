#ifndef PEASANTFORMER_Proto_Protocol
#define PEASANTFORMER_Proto_Protocol

/// @file

#include <Proto/Messages/Messages.h>
#include <Data/CircularBuffer/CircularBuffer.h>

/// Protocol class
class Protocol {
	private:
		int buffsize; ///< size of protocol buffer
		Messages *msgs; ///< messages pointer
		CircularBuffer<wchar_t> *circus; ///< circular buffer for current connection
	public:
		/// Protocol constructor
		Protocol(int buffsize, Messages *msgs);
		/// Protocol destructor
		~Protocol();
	public:
};

#endif
