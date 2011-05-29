#ifndef PEASANTFORMER_Proto_PlainIOHighLevel
#define PEASANTFORMER_Proto_PlainIOHighLevel

/// @file

#include <vector>
#include <string.h>
#include <wctype.h>

#include "PlainIO.h"

/// pio string; unicode-aware text representation in wchar_t.
class pio_string {
	private:
		std::vector<wchar_t> data; ///< wchar_t string data
		char *cstr; ///< c-string representation, including null-terminator
		wchar_t *wstr; ///< widechar-string representation, including null-terminator
		size_t cstr_len; ///< length of c-string representation
		size_t wstr_len; ///< length of widechar-string representation
	private:
		/// Widechar to internal data repesentation converter
		void wchar_to_data(
			const wchar_t *text, ///< widechar data
			int len = -1     ///< widechar length
		);
		/// Internal data to widechar representation conveter.
		/// writes to this->wstr
		void data_to_wchar();
		/// Initialization function
		void init();
	public:
		/// Dummy consructor.
		pio_string();
		/// Contructor from widechar string.
		pio_string(
			const wchar_t *source ///< source string to convert from
		);
		/// Constructor from refular c string.
		pio_string(
			const char *source ///< source string to convert from
		);
		/// Constructor from refular c string with limited length.
		pio_string(
			const char *source, ///< source string to convert from
			size_t len          ///< max length of convertin string
		);
		/// Constructor from unsigned char c string.
		pio_string(
			const unsigned char *source ///<  source string to covert from
		);

		/// Dummy destructor.
		~pio_string();
	public:
		/// Assignment operator.
		/// @return pio_string
		pio_string & operator=(
			pio_string const &r ///< right-hand expression
		);
		/// Subscripting operator.
		/// @return wchar_t
		wchar_t &operator[](
			size_t i ///< char index
		);
	public:
		/// Get internal data representation as it is.
		std::vector<wchar_t> get_data() const;
		/// Append supplied string to this one
		void append(
			const pio_string &oth ///< other string
		);
		/// @return length of the string
		size_t length();
		/// @return widechar string representation
		const wchar_t *w_str();
		/// @return c-string string representation
		const char *c_str();
		/// Clear string contents.
		void clear();
		/// Set string to a given value.
		void set(
			pio_string str ///< given string
		);
		/// Set string to a given value
		void set(
			const char * ///< given string
		);
		/// Set string to a given value
		void set(
			const unsigned char * ///< given string
		);
		/// Set string to a given value
		void set(
			const wchar_t * ///< given string
		);
		/// Filter string using predicate function
		void filter(
			bool (*predicate)(wchar_t) ///< given predicate function
		);
		/// Drop characters from left side of the string while predicate
		/// returns true.
		void strip_right(
			bool (*predicate)(wchar_t) ///< given predicate function
		);
		/// Drop characters from right side of the string while predicate
		/// return true. *NOT*IMPLEMENTED*YET*
		void strip_left(
			bool (*predicate)(wchar_t) ///< given predicate function
		);
		/// Same as filter(iswspace)
		void weedout_control();
};

/// operator compare-less for pio strings
/// @return bool
bool operator<(
	pio_string const& l, ///< [in] left string
	pio_string const& r ///< [in] right string
);
/// operator of string concat
/// @return resulting string
pio_string operator+(
	pio_string const& l, ///< [out] string append to
	pio_string const& r ///< [in] string append from
);
#endif

