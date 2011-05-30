#ifndef PEASANTFORMER_Proto_Messages
#define PEASANTFORMER_Proto_Messages

/// @file

#include <map>
#include <Data/Pstring/Pstring.h>

/// Messages class.
class Messages {
	private:
		std::map<Pstring,Pstring> messages; ///< messages data
	public:
		/// Constructs Messages class from filename with plaintext messages
		Messages(
			Pstring filename ///< [in] filepath
		); 
	public:
		/// @return string with specified message id
		const char *get(
			Pstring id ///< [in] message id
		);
		/// @return length of message with specified message id
		size_t length(
			Pstring id ///< [in] message id
		);
};

#endif
