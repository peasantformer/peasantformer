#include "CircularBuffer.h"

/// @file

void CircularBuffer::init() {
	ptr_begin = ptr_end = peek_ptr = write_ptr = 0;
	peek_fill = fill = 0;
	overflow_strategy = true;
}

void CircularBuffer::incr_strict(int *p) {
	(*p)++;
	if ((*p) > buffsize) (*p) = 1;
}

void CircularBuffer::decr_strict(int *p) {
	if ((*p) > 0) (*p)--;
}


void CircularBuffer::incr(int *p) {
	(*p)++;
	if ((*p) >= buffsize) (*p) = 0;
}

void CircularBuffer::decr(int *p) {
	(*p)--;
	if ((*p) < 0) (*p) = (buffsize - 1);
	printf("|||| %d\n",*p);
}

CircularBuffer::CircularBuffer() {
	init();
}

CircularBuffer::CircularBuffer(int buffsize) {
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

int CircularBuffer::forward_dist() {
	if (ptr_end > ptr_begin) {
		return ptr_end - ptr_begin;
	} else if (true) {
		return (buffsize - ptr_begin) + (ptr_end);
	} else {
		return fill;
	}
}

int CircularBuffer::backward_dist() {
	return buffsize - forward_dist();
}

int CircularBuffer::write(PString src, int length) {
	int len = 0;
	if (length < 0) {
		len = src.length();
	} else {
		len = length;
	}
	int i;
	for (i=0; i < len; i++) {
		if (write_ptr == ptr_begin && fill > 0 && overflow_strategy == false)
			break;
		data[write_ptr] = src[i];
		incr(&ptr_end);
		incr(&write_ptr);
		incr_strict(&fill);
		incr_strict(&peek_fill);
	}
	//peek_fill = fill;
	return i;
}

PString CircularBuffer::read(int length) {
	PString result;
	result.resize(length);
	
	for (int i=0; i < length; i++) {
		if (ptr_end == ptr_begin && fill == 0 && overflow_strategy == false)
			break;
		result[i] = data[ptr_begin];
		incr(&ptr_begin);
		decr_strict(&fill);
	}
	result.trim_right();
	return result;
}

wint_t CircularBuffer::read_ch() {
	if (ptr_end == ptr_begin && fill == 0 && overflow_strategy == false)
		return -1;
	wint_t result = data[ptr_begin];
	incr(&ptr_begin);
	decr_strict(&fill);
	return result;
}

PString CircularBuffer::peek(int length) {
	//peek_ptr = ptr_begin;
	//peek_fill = fill;
	PString result;
	result.resize(length);
	
	for (int i=0; i < length; i++) {
		printf("pukpukpuk %d %d %d\n",ptr_end,peek_ptr,peek_fill);
		if (ptr_end == peek_ptr && peek_fill == 0 && overflow_strategy == false)
			break;
		
		result[i] = data[peek_ptr];
		incr(&peek_ptr);
		decr_strict(&peek_fill);
	}
	result.trim_right();
	return result;
}

wint_t CircularBuffer::peek_ch() {
	//peek_ptr = ptr_begin;
	//peek_fill = fill;
	if (ptr_end == peek_ptr && peek_fill == 0 && overflow_strategy == false)
		return -1;
	wint_t result = data[peek_ptr];
	incr(&peek_ptr);
	decr_strict(&peek_fill);
	return result;
}

void CircularBuffer::reset_peek() {
	peek_ptr = ptr_begin;
	peek_fill = fill;
}

void CircularBuffer::scroll(int amount) {
	while (amount > 0) {
		incr(&ptr_begin);
//		incr(&ptr_end);
//		if (is_full()) incr(&ptr_end);
		amount--;
	}
	while (amount < 0) {
		decr(&ptr_begin);
		decr(&ptr_end);
		amount++;
	}
	//reset_peek();
}

void CircularBuffer::seek(int offset) {
	while (offset > 0) {
		incr(&peek_ptr);
		offset--;
	}
	while (offset < 0) {
		//printf("before: %d\n",peek_ptr);
		decr(&peek_ptr);
		//printf("after: %d\n",peek_ptr);
		
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


int CircularBuffer::get_fill() {
	return fill;
}


void CircularBuffer::dump() {
	for (int i=0; i < buffsize; i++) {
		char str[2];
		str[0] = data[i];
		str[1] = '\0';
		
		printf("#%03d [%s] ",(int)i,PString(str).c_str());
		if (i==write_ptr) printf("write ");
		if (i==peek_ptr) printf("peek ");
		if (i==ptr_begin) printf("begin ");
		if (i==ptr_end) printf("end ");
		printf("\n");
	}
	printf("fill: %d, buffsize: %d, peek fill: %d, peek ptr: %d\n",(int)fill,(int)buffsize,(int)peek_fill,(int)peek_ptr);
}
