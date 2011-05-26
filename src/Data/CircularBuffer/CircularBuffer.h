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
		CircularBuffer(); 
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

/* can't have this in different file, *sob*sob* */

template <class T>
CircularBuffer<T>::CircularBuffer(size_t buffsize) {
	this->buffsize = buffsize;
	this->buffer = (T*)malloc(this->buffsize * sizeof(T));
	memset(this->buffer,0,this->buffsize);
	this->ptr_begin = this->buffer;
	this->ptr_end = this->buffer;
	this->fill = 0;
	this->peek_fill = this->fill;
}

template<class T>
CircularBuffer<T>::CircularBuffer(const char *str) {
	this->buffsize = strlen(str);
	this->buffer = (char*)malloc(this->buffsize * sizeof (char));
	memcpy(this->buffer,str,buffsize);
	this->ptr_begin = this->buffer;
	this->ptr_end = this->buffer + buffsize;
	this->fill = buffsize;
	this->peek_fill = this->fill;
}

template<class T>
CircularBuffer<T>::CircularBuffer(const wchar_t *str) {
	this->buffsize = wcslen(str);
	this->buffer = (wchar_t*)malloc(this->buffsize * sizeof (wchar_t));
	memcpy(this->buffer,str,buffsize);
	this->ptr_begin = this->buffer;
	this->ptr_end = this->buffer + buffsize;
	this->fill = buffsize;
	this->peek_fill = this->fill;
}

template <class T>
CircularBuffer<T>::~CircularBuffer() {
	free(this->buffer);
}

template <class T>
size_t CircularBuffer<T>::forward_dist() {
	if (this->ptr_end > this->ptr_begin) {
		return this->ptr_end - this->ptr_begin;
	} else if (this->ptr_end < this->ptr_begin) {
		return ((this->buffer + this->buffsize) - this->ptr_begin) + (this->ptr_end - this->buffer);
	} else {
		return this->fill;
	}
}

template <class T>
size_t CircularBuffer<T>::backward_dist() {
	return this->buffsize - this->forward_dist();
}

template <class T>
int CircularBuffer<T>::write(const T *source, size_t len) {
	const T *s;
	size_t w = 0;

	for (s = source; len > 0; this->ptr_end++, s++, len--) {
		if (this->ptr_end > (this->buffer + this->buffsize - 1)) this->ptr_end = this->buffer;
		if (this->ptr_end == this->ptr_begin && this->fill > 0) { 
			break; 
		}
		if (source == NULL) {
			*this->ptr_end = '\0';	
		} else {
			*this->ptr_end = *s;
		}
		this->fill++;	
		w++;
	}
	this->peek_fill = this->fill;
	return w;
}
template <class T>
int CircularBuffer<T>::read(T *dest, size_t len) {
	T *d;
	size_t r = 0;

	for (d = dest; len > 0; this->ptr_begin++,d++,len--) {
		if (this->ptr_begin > (this->buffer + this->buffsize - 1)) this->ptr_begin = this->buffer;
		if (this->ptr_begin == this->ptr_end && this->fill == 0) { 
			break; 
		}
		if (dest != NULL) {
			*d = *this->ptr_begin;
		}
		this->fill--;
		r++;
	}
	this->peek_fill = this->fill;
	return r;
}

template <class T>
int CircularBuffer<T>::peek(size_t offset) {
	T *p = this->ptr_begin;
	if (offset >= this->buffsize) return -1;
	if ((p+offset) >= (this->buffer + this->buffsize)) {
		offset -= ((this->buffer + this->buffsize) - this->ptr_begin);
		p = this->buffer;
	}
	return *(p+offset);
}

template <class T>
int CircularBuffer<T>::peek_str(T *dest, size_t len) {
	T *p = this->ptr_begin;
	T *d;
	size_t r = 0;
	this->peek_fill = this->fill;

	for (d = dest; len > 0; p++,d++,len--) {
		if (p > (this->buffer + this->buffsize - 1)) p = this->buffer;
		if (p == this->ptr_end && this->fill == 0) { 
			break; 
		}
		if (dest != NULL) {
			*d = *p;
		}
		this->peek_fill--;
		r++;
	}
	return r;
}

template <class T>
int CircularBuffer<T>::is_eof() {
	return (this->fill == 0);
}
template <class T>
int CircularBuffer<T>::is_peek_eof() {
	return (this->peek_fill == 0);
}
template <class T>
int CircularBuffer<T>::is_full() {
	return (this->fill == this->buffsize);
}
template <class T>
void CircularBuffer<T>::clear() {
	this->ptr_begin = this->buffer;
	this->ptr_end = this->buffer;
	this->fill = 0;
	memset(this->buffer,0,this->buffsize);
}

#endif
