#ifndef PEASANTFORMER_Data_CircularBuffer
#define PEASANTFORMER_Data_CircularBuffer

/// @file

#include <vector>
#include <wctype.h>

#include <Proto/PlainIO/PlainIOHighLevel.h>

/// Unicode-aware string class
class PString {
	private:
		std::vector<wchar_t> data; ///< Textual data
		char *cstr;                ///< C-string representation of data
		wchar_t *wstr;             ///< widechar representation of data
		size_t cstr_len;           ///< length of cstr
		size_t wstr_len;           ///< length of wstr
	private:
		/// Converts widechar data to internal text representation
		void widechar_to_data(
			const wchar_t *text, ///< [in] widechar input string
			int length = -1      ///< [in] Length of wchar data
		);
		/// Converts internal text representation to widechar and store
		/// it in wstr.
		void data_to_widechar();
		/// Initializes things that a same for different constructors.
		void init();
	public:
		/// Dummy constructor.
		PString();
		/// Widechar constructor.
		PString(
			const wchar_t *source, ///< [in] widechar source data
			int length = -1        ///< [in] optional length of the string
		);
		/// C-string constructor.
		PString(
			const char *source, ///< [in] C source data
			int length = -1     ///< [in] optional length of the string
		);
		/// Unsgined C-sring constructor.
		PString(
			const unsigned char *source, ///< [in] C unsigned source data
			int length = -1              ///< [in] optional length of the string
		);
		/// Destructor
		~PString();
	public:
		/// Asignment operator
		/// @return PString refference
		PString & operator=(
			PString const &r ///< [in] right-hand expression
		);
		/// Subscripting operator
		/// @return wchar refference
		wchar_t & operator[](
			size_t i ///< [in] index
		);
	public:
		/// Get raw internal data representation.
		/// @return internal data vector
		std::vector<wchar_t> get_raw_data() const;
		/// String appending
		void append(
			const PString &r ///< [in] right-hand expression
		);
		/// Length of the text stored in the PString
		/// @return size_t length
		size_t length();
		/// @return C-string representation of data
		const char *c_str();
		/// @return widechar representation of data
		const wchar_t *w_str();
		/// Wipes PString
		void clear();
		/// Filters string using given predicate function
		void filter(
			bool (*predicate)(wchar_t) ///< [in] predicate function
		);
		/// Drops characters from left as long as predicate function
		/// returns true..
		void dropwhile_left(
			bool (*predicate)(wchar_t) ///< [in] predicate function
		);
		/// Drops characters from right as long as predicate function
		/// returns true..
		void dropwhile_right(
			bool (*predicate)(wchar_t) ///< [in] predicate function
		);
		/// Shortname for filter(iswcntrl)
		void filter_control();
		
};

/// Comparison less-then operator
/// @return bool
bool operator<(
	PString const& l, ///< [in] left string
	PString const& r  ///< [in] right string
);

/// Concatetipn operator.
/// @return new string
PString operator+ (
	PString const& l, ///< [out] string append to
	PString const& r  ///< [in] string append from
);

#endif
