#include "PlainIOUnix.h"

void pio_init() { 
	setlocale(LC_ALL,""); 
} 

size_t utf8stowcs(wchar_t *dest, char *source, size_t length) {
	return mbstowcs(dest,source,length);
}
size_t wcstoutf8s(char *dest, wchar_t *source, size_t length) {
	return wcstombs(dest,source,length);
}

