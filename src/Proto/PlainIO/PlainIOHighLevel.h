#ifndef PEASANTFORMER_Proto_PlainIOHighLevel
#define PEASANTFORMER_Proto_PlainIOHighLevel

#include <vector>
#include <string.h>
#include <wctype.h>

#include "PlainIO.h"

class pio_string {
	private:
		std::vector<wchar_t> data;
		char *cstr;
		wchar_t *wstr;
		size_t cstr_len;
		size_t wstr_len;
	private:
		void wchar_to_data(const wchar_t *);
		void data_to_wchar();
		void init();
	public:
		pio_string();
		pio_string(const wchar_t *);
		pio_string(const char *);
		pio_string(const unsigned char *);

		~pio_string();
	public:
		pio_string & operator=(pio_string const &r);
		wchar_t &operator[](size_t);
	public:
		std::vector<wchar_t> get_data() const;
		void append(const pio_string &);
		size_t length();
		const wchar_t *w_str();
		const char *c_str();
		void clear();
		void set(pio_string);
		void set(const char *);
		void set(const unsigned char *);
		void set(const wchar_t *);
		void filter(bool (*predicate)(wchar_t));
		void weedout_control();
};
bool operator<(pio_string const& l,pio_string const& r);
pio_string operator+(pio_string & l, pio_string const& r);
#endif

