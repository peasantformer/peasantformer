#ifndef PEASANTFORMER_Data_CircularBuffer
#define PEASANTFORMER_Data_CircularBuffer

/// @file

#include <Data/PString/PString.h>

/// Circular buffer implementation
class CircularBuffer {
	private:
		PString data;     ///< Buffer data
		int buffsize;  ///< Buffer size
		int ptr_begin; ///< Buffer start
		int ptr_end;   ///< Buffer end
		int write_ptr; ///< Buffer write pointer
		int peek_ptr;  ///< Peek pointer
		int fill;      ///< Fill count of the buffer
		int peek_fill; ///< Peek fill count of the buffer
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
		void incr_strict(int *p);
		/// Decriments buffer's fill count
		///
		/// \param [in,out] p is pointer to incriment
		void decr_strict(int *p);
		/// Incriments buffer's pointer
		///
		/// \param [in,out] p is pointer to incriment
		void incr(int *p);
		/// Decriments buffer's pointer
		///
		/// \param [in,out] p is pointer to decriment
		void decr(int *p);
	public:
		/// Dummy constructor
		CircularBuffer();
		
		/// Constructor with pre-defined buffer size.
		///
		/// \param [in] buffsize is buffer size
		CircularBuffer(int buffsize);
		
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
		int forward_dist();
		
		/// Returns distance from current pointer to begining of the buffer.
		///
		/// @return distance
		int backward_dist();
		
		/// Writes to buffer source char string
		///
		/// \param [in] src PString
		/// \param [in] length is optional length of source string
		/// @return length of written data
		int write(PString src, int length = -1);

		/// Reads from a buffer some string with given length
		///
		/// \param [in] length is length of the string to read
		/// @return PString
		PString read(int length);
		
		/// Reads single character from the bufer
		///
		/// @return wide-char code
		wint_t read_ch();
		
		/// Peeks from a buffer some string with given length, without
		/// removing read data from the buffer.
		///
		/// \param [in] length is length of the string to read
		/// @return PString
		PString peek(int length);
		
		/// Peeks single character from the bufer
		///
		/// @return wide-char code
		wint_t peek_ch();
		
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
		
		/// Resets peek pointer
		void reset_peek();
		
		/// Scroll buffer by given amount of chars
		///
		/// \param [in] amount is given amount of characters to scroll.
		void scroll(int amount);
		
		/// Scroll buffer to peek position
		void scroll_to_peek();
		
		/// Checks if buffer is full.
		///
		/// @return boolean
		bool is_full();
		
		
		/// Returns fill count
		///
		/// @return fill count
		int get_fill();
		
		/// Dumps buffer contetns on the screen. Useful for debuging.
		void dump();
};

#endif
