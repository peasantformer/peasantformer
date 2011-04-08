#ifndef CIRCULAR_BUFFER
#define CIRCULAR_BUFFER

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
		CircularBuffer(size_t buffsize){
			this->buffsize = buffsize;
			this->buffer = (char*)malloc(this->buffsize * sizeof (char));
			memset(this->buffer,0,this->buffsize);
			this->ptr_begin = this->buffer;
			this->ptr_end = this->buffer;
			this->fill = 0;
			this->peek_fill = this->fill;
		}
		CircularBuffer(const char *str){
			this->buffsize = strlen(str);
			this->buffer = (char*)malloc(this->buffsize * sizeof (char));
			memcpy(this->buffer,str,buffsize);
			this->ptr_begin = this->buffer;
			this->ptr_end = this->buffer + buffsize;
			this->fill = buffsize;
			this->peek_fill = this->fill;
		}
		~CircularBuffer() {
			free(this->buffer);
		}
	public:
		void raw_print() {
			printf("b=%ld e=%ld f=%ld     |     ",this->ptr_begin - this->buffer,this->ptr_end - this->buffer,this->fill);
			for (size_t i=0; i < this->buffsize; i++) {
				printf(".%c",this->buffer[i]);
			}
			printf("\n");
		}

		size_t forward_dist() {
			if (this->ptr_end > this->ptr_begin) {
				return this->ptr_end - this->ptr_begin;
			} else if (this->ptr_end < this->ptr_begin) {
				return ((this->buffer + this->buffsize) - this->ptr_begin) + (this->ptr_end - this->buffer);
			} else {
				return this->fill;
			}
		}
		
		size_t backward_dist() {
			return this->buffsize - this->forward_dist();
		}
		
		int write(const char *source, size_t len) {
			const char *s;
			size_t w = 0;

			for (this->ptr_end, s = source; len > 0; this->ptr_end++, s++, len--) {
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
		
		int read(char *dest, size_t len) {
			char *d;
			size_t r = 0;
			
			for (this->ptr_begin, d = dest; len > 0; this->ptr_begin++,d++,len--) {
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
		int peek(size_t offset) {
			char *p = this->ptr_begin;
			if (offset >= this->buffsize) return -1;
			if ((p+offset) >= (this->buffer + this->buffsize)) {
				offset -= ((this->buffer + this->buffsize) - this->ptr_begin);
				p = this->buffer;
			}
			return *(p+offset);
		}
		int peek_str(char *dest, size_t len) {
			char *p = this->ptr_begin;
			char *d;
			size_t r = 0;
			
			for (p, d = dest; len > 0; p++,d++,len--) {
				if (p > (this->buffer + this->buffsize - 1)) p = this->buffer;
				if (p == this->ptr_end && this->fill == 0) { 
					break; 
				}
				if (dest != NULL) {
					*d = *this->ptr_begin;
				}
				this->peek_fill--;
				r++;
			}
			return r;
		}
		int is_eof() {
			return (this->fill == 0);
		}
		int is_peek_eof() {
			return (this->peek_fill == 0);
		}
		int is_full() {
			return (this->fill == this->buffsize);
		}
		void clear() {
			this->ptr_begin = this->buffer;
			this->ptr_end = this->buffer;
			this->fill = 0;
			memset(this->buffer,0,this->buffsize);
		}
};
#endif
