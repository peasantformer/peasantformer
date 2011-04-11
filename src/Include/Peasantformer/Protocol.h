#ifndef PEASANTOFORMER_Protocol
#define PEASANTOFORMER_Protocol

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>

#else
#include <netinet/in.h>
#endif

#include <Peasantformer/Data/CircularBuffer.h>

const static size_t PNetProto_max_name_len = 4;
const static size_t PNetProto_max_message_len = 4;
const static size_t PNetProto_length_len = 2;
const static size_t PNetProto_max_packet_len = PNetProto_max_name_len + PNetProto_max_message_len + PNetProto_length_len * 2;


class PeasantMessage {
	private:
		char * name;
		size_t name_length;
		char * message;
		size_t message_length;
	public:
		PeasantMessage(const char *name, const char *message) {
			this->set_name(name);
			this->set_message(message);
		}
		~PeasantMessage() {
			free(this->name);
			free(this->message);
			this->name = NULL;
			this->message = NULL;
			this->name_length = 0;
			this->message_length = 0;
		}
		int set_name(const char *name) {
			/*
			size_t len = strnlen(name,PNetProto_max_name_len);
			this->name = (char *)malloc(len * sizeof (char));
			memcpy(this->name,name,len);
			this->name_length = len;
			
			*/
			return 0;
		}
		int set_message(const char *message) {
			/*
			size_t len = strnlen(message,PNetProto_max_message_len);
			this->message = (char *)malloc(len * sizeof (char));
			memcpy(this->message,message,len);
			this->message_length = len;
			
			*/
			return 0;
		}
	public:
		char * get_name(void) {
			return this->name;
		}
		size_t get_name_length(void) {
			return this->name_length;
		}
		char * get_message(void) {
			return this->message;
		}
		size_t get_message_length(void) {
			return this->message_length;
		}

};

class PeasantformerClientProtocol {
};

class PeasantformerServerProtocol {
};

class PeasantformerProtocol {
	public:
		CircularBuffer *in_buffer;
		CircularBuffer *out_buffer;
	
		PeasantformerServerProtocol *server;
		PeasantformerClientProtocol *client;
		

		
		PeasantformerProtocol(size_t in_buffer_size = 0, size_t out_buffer_size = 0) {
			if (in_buffer_size == 0)
				in_buffer_size = PNetProto_max_packet_len;
			if (out_buffer_size == 0)
				out_buffer_size = PNetProto_max_packet_len;
			
			this->in_buffer = new CircularBuffer(in_buffer_size);
			this->out_buffer = new CircularBuffer(out_buffer_size);
			this->server = new PeasantformerServerProtocol;
			this->client = new PeasantformerClientProtocol;
		}
		~PeasantformerProtocol() {
			delete this->server;
			delete this->client;
		}
	public:
		int pack(PeasantMessage msg, char *ptr) {
			if (ptr != NULL) return -1;
			
			size_t len 
			= PNetProto_length_len
			+ msg.get_name_length()
			+ 1 // for null separator
			+ msg.get_message_length()
			+ 1 // for null separator
			+ PNetProto_length_len;

			
			/*
			size_t nicklen = strnlen(msg.nick,this->max_nicklen);
			size_t msglen = strnlen(msg.message,this->max_msglen);
			size_t length = this->max_nicklen + msglen;
			char *p = result;
			const char *n = msg.nick;
			const char *m = msg.message;

			*p = (char)length;
			
			p++;
			for (;(n-msg.nick) < max_nicklen; n++,p++) {
				if ((n-msg.nick) < nicklen) {
					*p = *n;
				} else {
					*p = '\0';
				}
			}
			for (;(m-msg.message) < msglen; p++,m++) {
				*p = *m;
			}

			*p = (char)length;
			return 1 + length;
			*/
		}
};

#endif
