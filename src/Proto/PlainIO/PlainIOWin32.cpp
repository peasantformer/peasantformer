#include "PlainIOWin32.h"

/// @file

void pio_init() { 
	    setlocale(LC_ALL,""); 
		SetConsoleOutputCP(CP_UTF8);
} 

size_t pio_mbstowcs(wchar_t *dest, const char *source, size_t length) {
	return MultiByteToWideChar(CP_UTF8,0,source,-1,dest,length);
}
size_t pio_wcstombs(char *dest, const wchar_t *source, size_t length) {
	return WideCharToMultiByte(CP_UTF8, 0, source, -1, dest, length, NULL, NULL);;
}

