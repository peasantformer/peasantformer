#include "PlainIOUnix.h"

/// @file

void pio_init() { 
	setlocale(LC_ALL,""); 
} 

size_t pio_mbstowcs(wchar_t *dest, const char *source, size_t length) {
	return mbstowcs(dest,source,length);
}
size_t pio_wcstombs(char *dest, const wchar_t *source, size_t length) {
	return wcstombs(dest,source,length);
}

