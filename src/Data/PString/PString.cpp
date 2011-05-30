#include "PString.h"

/// @file

void PString::widechar_to_data(const wchar_t *text,int length) {
	if (length < 0)
		length = wcslen(text);
	for (int i=0; i < length; i++) {
		this->data.push_back(text[i]);
	}
}

void PString::data_to_widechar() {
	if (this->wstr != NULL) {
		delete[] this->wstr;
		this->wstr = NULL;
	}
	
	size_t length = data.size();
	
	this->wstr = new wchar_t[length +1];
	this->wstr_len = length + 1;
	
	std::vector<wchar_t>::iterator it;
	int i;
	for (i=0, it = data.begin(); it != data.end(); it++, i++) {
		this->wstr[i] = *it;
	}
	this->wstr[i] = '\0';
}

void PString::init() {
	this->cstr = NULL;
	this->wstr = NULL;
	this->cstr_len = 0;
	this->wstr_len = 0;
}



PString::PString() {
	this->init();
}

PString::PString(const wchar_t *source, int length) {
	this->init();
	this->widechar_to_data(source,length);
}

PString::PString(const char *source, int length) {
	this->init();
	wchar_t *wcs = NULL;
	pioh_mbtowc(&wcs,source,length);
	this->widechar_to_data(wcs,length);
	delete[] wcs;
}

PString::PString(const unsigned char *source, int length) {
	this->init();
	wchar_t *wcs = NULL;
	pioh_mbtowc(&wcs,(char*)source,length);
	this->widechar_to_data(wcs,length);
	delete[] wcs;
}

PString::~PString() {
	delete[] this->cstr;
	delete[] this->wstr;
}




PString & PString::operator=(PString const &r) {
	if (&r == this) return *this;
	this->data = r.data;
	return *this;
}

wchar_t & PString::operator[](size_t i) {
	return this->data[i];
}



std::vector<wchar_t> PString::get_raw_data() const {
	return this->data;
}

void PString::append(const PString &r) {
	std::vector<wchar_t> oth_data = r.get_raw_data();
	for (size_t i=0; i < oth_data.size(); i++) {
		this->data.push_back(oth_data[i]);
	}
}

size_t PString::length() {
	return this->data.size();
}

const char *PString::c_str() {
	if (this->cstr != NULL) {
		delete[] this->cstr;
		this->cstr = NULL;
	}
	this->data_to_widechar();
	pioh_wctomb(&this->cstr,this->wstr,data.size());
	return this->cstr;
}

const wchar_t *PString::w_str() {
	this->data_to_widechar();
	return this->wstr;
}

void PString::clear() {
	this->data.clear();
}

void PString::filter(bool (*predicate)(wchar_t)) {
	for (size_t i=0; i < data.size(); i++) {
		if ((*predicate)(data[i])) {
			this->data.erase(data.begin()+i);
		}
	}
}

void PString::dropwhile_left(bool (*predicate)(wchar_t)) {
	for (size_t i=0; i < data.size() && (*predicate)(data[i]); i++) {
		this->data.erase(data.begin()+i);
	}
}

void PString::dropwhile_right(bool (*predicate)(wchar_t)) {
	for (size_t i=data.size()-1; i >= 0 && (*predicate)(data[i]); i--) {
		this->data.erase(data.begin()+i);
	}
}

void PString::filter_control() {
	this->filter((bool(*)(wchar_t))iswcntrl);
}
