#ifndef PEASANTFORMER_Proto_Messages
#define PEASANTFORMER_Proto_Messages

/// @file

#include <map>
#include <Data/PString/PString.h>

/// Messages class.
class Messages {
	private:
		std::map<PString,PString> messages; ///< messages data
	public:
		
		/// Constructs Messages class from filename with plaintext messages
		///
		/// \param [in] filename is filepath
		Messages(PString filename); 
	public:
		
		/// Get string with given id
		///
		/// \param [in] id is message id
		/// @return string with specified message id
		const char *get(PString id);
		
		/// Get length of the message with specified id
		///
		/// \param [in] id is message id
		/// @return length of message with specified message id
		size_t length(PString id);
};

#endif
