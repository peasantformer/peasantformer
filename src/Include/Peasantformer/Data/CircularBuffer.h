#ifndef PEASANTFORMER_Data_CircularBuffer
#define PEASANTFORMER_Data_CircularBuffer

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

class CircularBuffer {
	private:
		char *buffer;
		char *ptr_begin;
		char *ptr_end;
		char *ptr;
		size_t fill;
		size_t peek_fill;
		size_t buffsize;
	public:
		CircularBuffer(size_t buffsize);
		CircularBuffer(const char *str);
		~CircularBuffer();
	public:
		size_t forward_dist();
		size_t backward_dist();
		int write(const char *source, size_t len);
		int read(char *dest, size_t len);
		int peek(size_t offset);
		int peek_str(char *dest, size_t len);
		int is_eof();
		int is_peek_eof();
		int is_full();
		void clear();
};
#endif
