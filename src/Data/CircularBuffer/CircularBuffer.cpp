#include "CircularBuffer.h"

/// @file


CircularBuffer::CircularBuffer(size_t buffsize) {
	this->buffsize = buffsize;
	this->buffer = (wchar_t*)malloc(this->buffsize * sizeof(wchar_t));
	memset(this->buffer,0,this->buffsize);
	this->ptr_begin = this->buffer;
	this->ptr_end = this->buffer;
	this->fill = 0;
	this->peek_fill = this->fill;
}

CircularBuffer::CircularBuffer(const char *str) {
	pio_string pstr(str);
	this->buffsize = pstr.length();
	this->buffer = (wchar_t*)malloc(this->buffsize * sizeof (wchar_t));
	memcpy(this->buffer,pstr.w_str(),buffsize);
	this->ptr_begin = this->buffer;
	this->ptr_end = this->buffer + buffsize;
	this->fill = buffsize;
	this->peek_fill = this->fill;
}

CircularBuffer::CircularBuffer(const wchar_t *str) {
	this->buffsize = wcslen(str);
	this->buffer = (wchar_t*)malloc(this->buffsize * sizeof (wchar_t));
	memcpy(this->buffer,str,buffsize);
	this->ptr_begin = this->buffer;
	this->ptr_end = this->buffer + buffsize;
	this->fill = buffsize;
	this->peek_fill = this->fill;
}

CircularBuffer::~CircularBuffer() {
	free(this->buffer);
}

size_t CircularBuffer::forward_dist() {
	if (this->ptr_end > this->ptr_begin) {
		return this->ptr_end - this->ptr_begin;
	} else if (this->ptr_end < this->ptr_begin) {
		return ((this->buffer + this->buffsize) - this->ptr_begin) + (this->ptr_end - this->buffer);
	} else {
		return this->fill;
	}
}

size_t CircularBuffer::backward_dist() {
	return this->buffsize - this->forward_dist();
}

int CircularBuffer::write(const wchar_t *source, size_t len) {
	const wchar_t *s;
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

int CircularBuffer::read(wchar_t *dest, size_t len) {
	wchar_t *d;
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

int CircularBuffer::peek(size_t offset) {
	wchar_t *p = this->ptr_begin;
	if (offset >= this->buffsize) return -1;
	if ((p+offset) >= (this->buffer + this->buffsize)) {
		offset -= ((this->buffer + this->buffsize) - this->ptr_begin);
		p = this->buffer;
	}
	return *(p+offset);
}

int CircularBuffer::peek_str(wchar_t *dest, size_t len) {
	wchar_t *p = this->ptr_begin;
	wchar_t *d;
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

int CircularBuffer::is_eof() {
	return (this->fill == 0);
}
int CircularBuffer::is_peek_eof() {
	return (this->peek_fill == 0);
}
int CircularBuffer::is_full() {
	return (this->fill == this->buffsize);
}
void CircularBuffer::clear() {
	this->ptr_begin = this->buffer;
	this->ptr_end = this->buffer;
	this->fill = 0;
	memset(this->buffer,0,this->buffsize);
}

