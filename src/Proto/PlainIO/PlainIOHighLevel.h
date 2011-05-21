#ifndef PEASANTFORMER_Proto_PlainIOHighLevel
#define PEASANTFORMER_Proto_PlainIOHighLevel

#include <vector>

#include "PlainIO.h"

class pio_string {
	private:
		std::vector<wchar_t> data;
		char *cstr;
		wchar_t *wstr;
	private:
		void wchar_to_data(const wchar_t *);
		void data_to_wchar();
		void init();
	public:
		pio_string();
		pio_string(const wchar_t *);
		pio_string(char *);

		~pio_string();
	public:
		size_t length();
		wchar_t &operator[](size_t);
		const wchar_t *w_str();
		const char *c_str();
		void clear();
};

#endif

