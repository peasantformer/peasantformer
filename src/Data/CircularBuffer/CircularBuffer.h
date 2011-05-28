#ifndef PEASANTFORMER_Data_CircularBuffer
#define PEASANTFORMER_Data_CircularBuffer

/// @file

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>

/// Simple two-way circular buffer.
template <class T>
class CircularBuffer {
	private:
		T *buffer; ///< actual buffer
		T *ptr_begin; ///< pointer to begining of the bufer
		T *ptr_end; ///< pointer to end of the buffer
		T *ptr; ///< current position pointer
		size_t fill; ///< fill count of the buffer
		size_t peek_fill; ///< peek fill count of the buffer
		size_t buffsize; ///< size of the buffer
	public:
		/// Creates dummy buffer.
		CircularBuffer(); 
		/// Creates buffer with size buffsize.
		CircularBuffer(
			size_t buffsize ///< [in] size of circular buffer
		);
		/// Creates buffer from char string.
		CircularBuffer(
			const char *str ///< [in] string to circulate. Buffersize will be adjusted to length of this string.
		);
		/// Creates buffer from wchar_t string.
		CircularBuffer(
			const wchar_t *str ///< [in] wchar_t string to circulate. Buffsize will be adjusted to length of this strig.
		);
		~CircularBuffer();
	public:
		/// Returns distance to end of the buffer.
		/// @return distance to end of the buffer
		size_t forward_dist();
		/// Returns distance to begining of the buffer.
		/// @return distance to begining of the buffer
		size_t backward_dist();
		/// Writes string to the buffer.
		/// @return writed chars count
		int write(
			const T *source, ///< [in] source string to write into the buffer
			size_t len ///< [in] source string length
		);
		/// Reads contents of buffer.
		/// @return number of read characters
		int read(
			T *dest, ///< [out] dest destination string to write from the buffer.
			size_t len ///< [in] length of this string
		);
		/// Peeks character in the buffer without moving main pointers.
		/// @return peeked character code
		int peek(
			size_t offset ///< [in] offset from which get chatacter to peek
		);
		/// Peeks a len number of characters to dest string.
		/// @return number of peeked characters
		int peek_str(
			T *dest,  ///< [out] dest destination string to peek from the buffer
			size_t len ///< [in] len number of characters to peek
		);
		/// Checks if end of buffer where reached.
		/// @return non-zero if true
		int is_eof();
		/// Checks if end of buffer where reached by peek/peek_str routines.
		/// @return non-zero if true
		int is_peek_eof();
		/// Checks if buffer is fully loaded.
		/// @return non-zero if true
		int is_full();
		/// Clears the buffer.
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
