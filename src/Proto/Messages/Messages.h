#ifndef PEASANTFORMER_Proto_Messages
#define PEASANTFORMER_Proto_Messages

#include <map>
#include <Proto/PlainIO/PlainIO.h>
#include <Proto/PlainIO/PlainIOHighLevel.h>

class Messages {
	private:
		std::map<pio_string,pio_string>messages;
	public:
		Messages(pio_string filename);
	public:
		const char *get(pio_string id);
		const char *getln(pio_string id);
		size_t length(pio_string id);
};

#endif
