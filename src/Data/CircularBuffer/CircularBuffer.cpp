#include "CircularBuffer.h"

/// @file

void CircularBuffer::init() {
	ptr_begin = ptr_end = peek_ptr = 0;
	peek_fill = fill = 0;
	overflow_strategy = true;
}

void CircularBuffer::incr_strict(size_t *p) {
	(*p)++;
	if ((*p) > buffsize) *p = 1;
}

void CircularBuffer::decr_strict(size_t *p) {
	if ((*p) > 0) (*p)--;
}


void CircularBuffer::incr(size_t *p) {
	(*p)++;
	if ((*p) >= buffsize) *p = 0;
}

void CircularBuffer::decr(size_t *p) {
	(*p)--;
	if ((*p) < 0) *p = buffsize - 1;
}

CircularBuffer::CircularBuffer() {
	init();
}

CircularBuffer::CircularBuffer(size_t buffsize) {
	init();
	this->buffsize = buffsize;
	data.resize(buffsize);
}

CircularBuffer::CircularBuffer(const char *source) {
	init();
	buffsize = strlen(source);
	data = PString(source);
}

CircularBuffer::CircularBuffer(const wchar_t *source) {
	init();
	buffsize = wcslen(source);
	data = PString(source);
}

CircularBuffer::~CircularBuffer() {
}




void CircularBuffer::clear() {
	data.clear();
	bool strategy = overflow_strategy;
	init();
	overflow_strategy = strategy;
}

void CircularBuffer::disable_overflow() {
	overflow_strategy = false;
}

size_t CircularBuffer::forward_dist() {
	if (ptr_end > ptr_begin) {
		return ptr_end - ptr_begin;
	} else if (true) {
		return (buffsize - ptr_begin) + (ptr_end);
	} else {
		return fill;
	}
}

size_t CircularBuffer::backward_dist() {
	return buffsize - forward_dist();
}

size_t CircularBuffer::write(PString src, int length) {
	size_t len = 0;
	if (length < 0) {
		len = src.length();
	} else {
		len = length;
	}
	size_t i;
	for (i=0; i < len; i++) {
		if (ptr_end == ptr_begin && fill > 0 && overflow_strategy == false)
			break;
		data[ptr_end] = src[i];
		incr(&ptr_end);
		incr_strict(&fill);
	}
	peek_fill = fill;
	return i;
}

PString CircularBuffer::read(size_t length) {
	PString result;
	result.resize(length);
	
	for (size_t i=0; i < length; i++) {
		if (ptr_end == ptr_begin && fill == 0 && overflow_strategy == false)
			break;
		result[i] = data[ptr_begin];
		incr(&ptr_begin);
		decr_strict(&fill);
	}
	result.trim_right();
	return result;
}

PString CircularBuffer::peek(size_t length) {
	peek_ptr = ptr_begin;
	peek_fill = fill;
	PString result;
	result.resize(length);
	
	for (size_t i=0; i < length; i++) {
		if (ptr_end == peek_ptr && peek_fill == 0 && overflow_strategy == false)
			break;
		result[i] = data[peek_ptr];
		incr(&peek_ptr);
		decr_strict(&peek_fill);
	}
	result.trim_right();
	return result;
}




void CircularBuffer::seek(int offset) {
	while (offset > 0) {
		decr(&peek_ptr);
		offset--;
	}
	while (offset < 0) {
		incr(&peek_ptr);
		offset++;
	}
}




bool CircularBuffer::is_eof() {
	return (fill == 0);
}

bool CircularBuffer::is_peek_eof() {
	return (peek_fill == 0);
}

bool CircularBuffer::is_full() {
	return (fill == buffsize);
}


void CircularBuffer::dump() {
	for (size_t i=0; i < buffsize; i++) {
		char str[2];
		str[0] = data[i];
		str[1] = '\0';
		
		printf("#%03d [%s] ",(int)i,PString(str).c_str());
		if (i==peek_ptr) printf("peek ");
		if (i==ptr_begin) printf("begin ");
		if (i==ptr_end) printf("end ");
		printf("\n");
	}
	printf("fill: %d, buffsize: %d, fdist: %d, bdit: %d\n",(int)fill,(int)buffsize,(int)forward_dist(),(int)backward_dist());
}
