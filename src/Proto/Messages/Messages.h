#ifndef PEASANTFORMER_Proto_Messages
#define PEASANTFORMER_Proto_Messages

/// @file

#include <map>
#include <Proto/PlainIO/PlainIO.h>
#include <Proto/PlainIO/PlainIOHighLevel.h>

/// Messages class.
class Messages {
	private:
		std::map<pio_string,pio_string>messages; ///< messages data
	public:
		/// Constructs Messages class from filename with plaintext messages
		Messages(
			pio_string filename ///< [in] filepath
		); 
	public:
		/// @return string with specified message id
		const char *get(
			pio_string id ///< [in] message id
		);
		/// @return length of message with specified message id
		size_t length(
			pio_string id ///< [in] message id
		);
};

#endif
