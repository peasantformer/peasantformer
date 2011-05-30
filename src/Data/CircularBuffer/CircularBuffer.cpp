#include "CircularBuffer.h"

/// @file

void CircularBufffer::init() {
	ptr_begin = ptr_end = ptr = 0;
	peek_fill = fill = 0;
	overflow_strategy = true;
}

CircularBufffer::CircularBufffer() {
	init();
}

CircularBufffer::CircularBufffer(size_t buffsize) {
	init();
	this->buffsize = buffsize;
	data.resize(buffsize);
}

CircularBufffer::CircularBufffer(const char *source) {
	init();
	buffsize = strlen(source);
	data = PString(source);
}

CircularBufffer::CircularBufffer(const wchar_t *source) {
	init();
	buffsize = wcslen(source);
	data = PString(source);
}

CircularBufffer::~CircularBufffer() {
}




void CircularBufffer::clear() {
	data.clear();
	init();
}

size_t CircularBufffer::forward_dist() {
	if (ptr_end > ptr_begin) {
		return ptr_end - ptr_begin;
	} else if (true) {
		return (buffsize - ptr_begin) + (ptr_end);
	} else {
		return fill;
	}
}

size_t CircularBufffer::backward_dist() {
	return buffsize - forward_dist();
}

size_t CircularBufffer::write(PString src, int length) {
	size_t len = 0;
	if (length < 0) {
		len = src.length();
	} else {
		len = length;
	}
	size_t i;
	for (i=0; i < len; i++,ptr_end++,fill++) {
		if (ptr_end >= buffsize) ptr_end = 0;
		if (ptr_end == ptr_begin && fill > 0 && overflow_strategy == false) {
			break;
		}
		data[ptr_end] = src[i];
	}
	if (ptr_end >= buffsize) ptr_end = 0;
	peek_fill = fill;
	return i;
}

wint_t CircularBufffer::read_ch() {
	return 0;
}

size_t CircularBufffer::read_str(wchar_t *dest, size_t length) {
	return 0;
}

wint_t CircularBufffer::peek_ch(size_t offset) {
	return 0;
}

size_t CircularBufffer::peek_str(wchar_t *dest, size_t length) {
	return 0;
}




void CircularBufffer::seek(int offset) {
}




bool CircularBufffer::is_eof() {
	return false;
}

bool CircularBufffer::is_peek_eof() {
	return false;
}

bool CircularBufffer::is_full() {
	return false;
}


void CircularBufffer::dump() {
	for (size_t i=0; i < buffsize; i++) {
		char str[2];
		str[0] = data[i];
		str[1] = '\0';
		
		printf("#%03d [%s] ",i,PString(str).c_str());
		if (i==ptr) printf("< ");
		if (i==ptr_begin) printf("begin ");
		if (i==ptr_end) printf("end ");
		printf("\n");
	}
}
