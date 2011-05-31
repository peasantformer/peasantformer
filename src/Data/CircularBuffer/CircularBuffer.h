#ifndef PEASANTFORMER_Data_CircularBuffer
#define PEASANTFORMER_Data_CircularBuffer

/// @file

#include <Data/PString/PString.h>

/// Circular buffer implementation
class CircularBuffer {
	private:
		PString data;     ///< Buffer data
		size_t buffsize;  ///< Buffer size
		size_t ptr_begin; ///< Buffer start
		size_t ptr_end;   ///< Buffer end
		size_t peek_ptr;  ///< Peek pointer
		size_t fill;      ///< Fill count of the buffer
		size_t peek_fill; ///< Peek fill count of the buffer
		bool overflow_strategy; ///< Determines the overflow strategy;
		                        ///< that means if it is true - buffer
		                        ///< will be overwriten on overflow;
		                        ///< if false - writing will be interrupted
		                        ///< on overflow.
	private:
		/// Initialization function, contains shared to all constructors
		/// routines.
		void init();
		/// Incriments buffer's fill count
		///
		/// \param [in,out] p is pointer to incriment
		void incr_strict(size_t *p);
		/// Decriments buffer's fill count
		///
		/// \param [in,out] p is pointer to incriment
		void decr_strict(size_t *p);
		/// Incriments buffer's pointer
		///
		/// \param [in,out] p is pointer to incriment
		void incr(size_t *p);
		/// Decriments buffer's pointer
		///
		/// \param [in,out] p is pointer to decriment
		void decr(size_t *p);
	public:
		/// Dummy constructor
		CircularBuffer();
		
		/// Constructor with pre-defined buffer size.
		///
		/// \param [in] buffsize is buffer size
		CircularBuffer(size_t buffsize);
		
		/// Constructor with pre-defined source string.
		///
		/// \param [in] source is source char string
		CircularBuffer(const char *source);
		
		/// Constructor with pre-defined widechar string.
		///
		/// \param [in] source is source widechar string
		CircularBuffer(const wchar_t *source);
		
		/// Dummy destructor.
		~CircularBuffer();
	public:
		/// Clears buffer contetns.
		void clear();
		
		/// Sets buffer strategy to non-overflow-tolerant one
		void disable_overflow();
		
		/// Returns distance from current pointer to end of the buffer.
		///
		/// @return distance
		size_t forward_dist();
		
		/// Returns distance from current pointer to begining of the buffer.
		///
		/// @return distance
		size_t backward_dist();
		
		/// Writes to buffer source char string
		///
		/// \param [in] src PString
		/// \param [in] length is optional length of source string
		/// @return length of written data
		size_t write(PString src, int length = -1);

		/// Reads from a buffer some string with given length
		///
		/// \param [in] length is length of the string to read
		/// @return PString
		PString read(size_t length);
		
		/// Peeks from a buffer some string with given length, without
		/// removing read data from the buffer.
		///
		/// \param [in] length is length of the string to read
		/// @return PString
		PString peek(size_t length);
		
		/// Seeks current pointer to a speified position.
		///
		/// \param [in] offset is offset from current pointer position
		void seek(int offset);
		
		/// Checks if end of buffer were reached by any of read* operations.
		///
		/// @return boolean
		bool is_eof();
		
		/// Checks if end of buffer were reached by any of peek* operations.
		///
		/// @return boolean
		bool is_peek_eof();
		
		/// Checks if buffer is full.
		///
		/// @return boolean
		bool is_full();
		
		/// Dumps buffer contetns on the screen. Useful for debuging.
		void dump();
};

#endif
