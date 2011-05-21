#include "PlainIOHighLevel.h"

void pio_string::wchar_to_data(const wchar_t *text) {
	size_t length = wcslen(text);
	for (size_t i=0; i < length; i++) {
		this->data.push_back(text[i]);
	}
}


void pio_string::data_to_wchar() {
	if (this->wstr != NULL) delete this->wstr;
	
	size_t length = data.size();

	this->wstr = new wchar_t[length + 1];

	std::vector<wchar_t>::iterator it;
	int i;
	for (it = data.begin() , i=0; it != data.end(); it++,i++) {
		this->wstr[i] = *it;
	}
	this->wstr[i] = '\0';
}
void pio_string::init() {
	this->cstr = NULL;
	this->wstr = NULL;
}

pio_string::pio_string() {
	this->init();
}


pio_string::pio_string(const wchar_t *source) {
	this->init();
	this->wchar_to_data(source);
}

pio_string::~pio_string() {
	delete this->cstr;
	delete this->wstr;
}

void pio_string::clear() {
	this->data.clear();
	delete this->cstr;
	delete this->wstr;
	this->cstr = NULL;
	this->wstr = NULL;
}


pio_string::pio_string(char *source) {
	this->init();

	size_t buffsize = 0;
	buffsize = utf8stowcs(NULL,source,0);

	wchar_t *wch = new wchar_t[buffsize+1];
	
	utf8stowcs(wch,source,buffsize);
	wch[buffsize] = '\0';

	this->wchar_to_data(wch);

	delete wch;
}

const char *pio_string::c_str() {
	this->data_to_wchar();
	if (this->cstr != NULL) delete this->cstr;

	this->data_to_wchar();

	size_t buffsize = 0;
	buffsize = wcstoutf8s(NULL,this->wstr,0);
	
	this->cstr = new char[buffsize+1];

	wcstoutf8s(this->cstr,this->wstr,buffsize);
	this->cstr[buffsize] = '\0';

	return this->cstr;
}
