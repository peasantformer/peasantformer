#ifndef PEASANTFORMER_Proto_PlainIO
#define PEASANTFORMER_Proto_PlainIO

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#ifdef _WIN32

#include <windows.h>

#endif

void pio_init();
size_t utf8stowcs(wchar_t *, const char *,size_t);
size_t wcstoutf8s(char *, const wchar_t *,size_t);


#endif

