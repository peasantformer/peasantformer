#ifndef PEASANTFORMER_Data_CircularBuffer
#define PEASANTFORMER_Data_CircularBuffer

/// @file

#include <Data/PString/PString.h>

/// Circular buffer implementation
class CircularBufffer {
	private:
		PString data;     ///< Buffer data
		size_t buffsize;  ///< Buffer size
		size_t ptr_begin; ///< Buffer start
		size_t ptr_end;   ///< Buffer end
		size_t ptr;       ///< Buffer current pointer
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
	public:
		/// Dummy constructor
		CircularBufffer();
		
		/// Constructor with pre-defined buffer size.
		///
		/// \param [in] buffsize is buffer size
		CircularBufffer(size_t buffsize);
		
		/// Constructor with pre-defined source string.
		///
		/// \param [in] source is source char string
		CircularBufffer(const char *source);
		
		/// Constructor with pre-defined widechar string.
		///
		/// \param [in] source is source widechar string
		CircularBufffer(const wchar_t *source);
		
		/// Dummy destructor.
		~CircularBufffer();
	public:
		/// Clears buffer contetns.
		void clear();
		
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
		/// \param [in] source PString
		/// \param [in] length is optional length of source string
		/// @return length of written data
		size_t write(PString src, int length = -1);

		
		/// Reads from a buffer single character
		///
		/// @return character code
		wint_t read_ch();
		
		/// Reads from a buffer some string with given length
		///
		/// \param [out] dest is destination buffer
		/// \param [in] length is length of the string to read
		/// @return length of read data
		size_t read_str(wchar_t *dest, size_t length);
		
		/// Peeks from a buffer single character on given offset from
		/// current pointer position.
		///
		/// \param [in] offset is offset of peeking character
		/// @return character code
		wint_t peek_ch(size_t offset);
		
		/// Peeks from a buffer some string with given length, without
		/// removing read data from the buffer.
		///
		/// \param [out] dest is destination buffer
		/// \param [in] length is length of the string to read
		/// @return length of peeked string
		size_t peek_str(wchar_t *dest, size_t length);
		
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
