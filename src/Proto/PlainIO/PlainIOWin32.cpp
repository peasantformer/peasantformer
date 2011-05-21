#include "PlainIOWin32.h"

void pio_init() { 
	    setlocale(LC_ALL,""); 
		SetConsoleOutputCP(CP_UTF8);
} 

size_t utf8stowcs(wchar_t *dest, char *source, size_t length) {
	return MultiByteToWideChar(CP_UTF8,0,source,-1,dest,length);
}
size_t wcstoutf8s(char *dest, wchar_t *source, size_t length) {
	return WideCharToMultiByte(CP_UTF8, 0, source, -1, dest, length, NULL, NULL);;
}

