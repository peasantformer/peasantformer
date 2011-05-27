#include "PlainIOHighLevel.h"

void pio_string::wchar_to_data(const wchar_t *text) {
	size_t length = wcslen(text);
	for (size_t i=0; i < length; i++) {
		this->data.push_back(text[i]);
	}
}


void pio_string::data_to_wchar() {
	if (this->wstr != NULL) {
		delete[] this->wstr;
		this->wstr = NULL;
	}
	
	size_t length = data.size();

	this->wstr = new wchar_t[length + 1];
	this->wstr_len = length + 1;

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
	this->cstr_len = 0;
	this->wstr_len = 0;
}

pio_string::pio_string() {
	this->init();
}


pio_string::pio_string(const wchar_t *source) {
	this->init();
	this->wchar_to_data(source);
}

pio_string::~pio_string() {
	if (this->cstr != NULL) {
		delete[] this->cstr;
		this->cstr = NULL;
	}
	if (this->wstr != NULL) {
		delete[] this->wstr;
		this->wstr = NULL;
	}
}

void pio_string::clear() {
	this->data.clear();
	delete[] this->cstr;
	delete[] this->wstr;
	this->cstr = NULL;
	this->wstr = NULL;
}


pio_string::pio_string(const char *source) {
	this->init();

	size_t buffsize = 0;
	buffsize = utf8stowcs(NULL,source,0);

	wchar_t *wch = new wchar_t[buffsize+1];
	
	utf8stowcs(wch,source,buffsize);
	wch[buffsize] = '\0';

	this->wchar_to_data(wch);

	delete[] wch;
	wch = NULL;
}

pio_string::pio_string(const unsigned char *source) {
	this->init();

	size_t buffsize = 0;
	buffsize = utf8stowcs(NULL,(const char*)source,0);

	wchar_t *wch = new wchar_t[buffsize+1];
	
	utf8stowcs(wch,(const char*)source,buffsize);
	wch[buffsize] = '\0';

	this->wchar_to_data(wch);

	delete[] wch;
	wch = NULL;
}

size_t pio_string::length() {
	return this->data.size();
}

wchar_t &pio_string::operator[](size_t i) {
	return this->data[i];
}

pio_string & pio_string::operator=(pio_string const &r) {
	if (&r == this) return *this;
	this->data = r.data;
	this->cstr = NULL;
	this->wstr = NULL;
	if (r.cstr != NULL) {
		this->cstr = new char[cstr_len];
		strncpy(this->cstr,r.cstr,cstr_len);
	}
	if (r.wstr != NULL) {
		this->wstr = new wchar_t[wstr_len];
		wcsncpy(this->wstr,r.wstr,wstr_len);
	}
	return *this;
}

const wchar_t *pio_string::w_str()  {
	this->data_to_wchar();
	return this->wstr;
}
const char *pio_string::c_str() {
	this->data_to_wchar();
	if (this->cstr != NULL) {
		 delete[] this->cstr;
		 this->cstr = NULL;
	 }

	this->data_to_wchar();

	size_t buffsize = 0;
	buffsize = wcstoutf8s(NULL,this->wstr,0);
	
	this->cstr = new char[buffsize+1];
	this->cstr_len = buffsize + 1;

	wcstoutf8s(this->cstr,this->wstr,buffsize);
	this->cstr[buffsize] = '\0';

	return this->cstr;
}

void pio_string::filter(bool (*predicate)(wchar_t)) {
	for (size_t i=0; i < this->length(); i++) {
		if ((*predicate)(this->data[i])) {
			this->data.erase(this->data.begin()+i);
			i--;
		}
	}
}

void pio_string::strip_right(bool (*predicate)(wchar_t)) {
	for (size_t i = (this->length() - 1); i >= 0 && (*predicate)(this->data[i]); i--) {
		this->data.erase(this->data.begin()+i);
	}
}

void pio_string::set(pio_string str) {
	std::vector<wchar_t> oth_data = str.get_data();
	this->data = oth_data;
}
void pio_string::set(const char *str) {
	this->set(pio_string(str));
}
void pio_string::set(const unsigned char *str) {
	this->set(pio_string(str));
}
void pio_string::set(const wchar_t *str) {
	this->set(pio_string(str));
}

void pio_string::weedout_control() {
	this->filter((bool(*)(wchar_t))iswcntrl);
}
std::vector<wchar_t> pio_string::get_data() const {
	return this->data;
}

void pio_string::append(const pio_string &oth) {
	std::vector<wchar_t> oth_data = oth.get_data();
	for (size_t i=0; i < oth_data.size(); i++) {
		wchar_t ch = oth_data[i];
		this->data.push_back(ch);
	}
}

bool operator<(pio_string const& l,pio_string const& r) {
	return l.get_data() < r.get_data();
}

pio_string operator+(pio_string & l, pio_string const& r) {
	l.append(r);
	return l;
}


