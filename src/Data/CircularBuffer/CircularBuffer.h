#ifndef PEASANTFORMER_Data_CircularBuffer
#define PEASANTFORMER_Data_CircularBuffer

/// @file

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <Proto/PlainIO/PlainIOHighLevel.h>

/// Simple two-way circular buffer.
class CircularBuffer {
	private:
		wchar_t *buffer;    ///< actual buffer
		wchar_t *ptr_begin; ///< pointer to begining of the bufer
		wchar_t *ptr_end;   ///< pointer to end of the buffer
		wchar_t *ptr;       ///< current position pointer
		size_t fill;        ///< fill count of the buffer
		size_t peek_fill;   ///< peek fill count of the buffer
		size_t buffsize;    ///< size of the buffer
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
			const wchar_t *source, ///< [in] source string to write into the buffer
			size_t len ///< [in] source string length
		);
		/// Write C string to the buffer, converting it to wchar_t
		/// @return writed chars count
		int write(
			const char *source, ///< [in] source C string to write into the buffer
			size_t len          ///< [in] source string length
		);
		/// Reads contents of buffer.
		/// @return number of read characters
		int read(
			wchar_t *dest, ///< [out] dest destination string to write from the buffer.
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
			wchar_t *dest,  ///< [out] dest destination string to peek from the buffer
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

#endif
