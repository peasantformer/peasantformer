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
		///
		/// \param [in] filename is filepath
		Messages(Pstring filename); 
	public:
		
		/// Get string with given id
		///
		/// \param [in] id is message id
		/// @return string with specified message id
		const char *get(Pstring id);
		
		/// Get length of the message with specified id
		///
		/// \param [in] id is message id
		/// @return length of message with specified message id
		size_t length(Pstring id);
};

#endif
