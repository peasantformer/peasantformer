#ifndef PEASANTFORMER_Data_CircularBuffer
#define PEASANTFORMER_Data_CircularBuffer

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

template <class T>
class CircularBuffer {
	private:
		T *buffer;
		T *ptr_begin;
		T *ptr_end;
		T *ptr;
		size_t fill;
		size_t peek_fill;
		size_t buffsize;
	public:
		CircularBuffer(size_t buffsize);
		CircularBuffer(const char *str);
		CircularBuffer(const wchar_t *str);
		~CircularBuffer();
	public:
		size_t forward_dist();
		size_t backward_dist();
		int write(const T *source, size_t len);
		int read(T *dest, size_t len);
		int peek(size_t offset);
		int peek_str(T *dest, size_t len);
		int is_eof();
		int is_peek_eof();
		int is_full();
		void clear();
};

#endif
