#ifndef PEASANTFORMER_Proto_PlainIO
#define PEASANTFORMER_Proto_PlainIO

#include <locale.h>

#ifdef _WIN32

#include <windows.h>

#else

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#endif

void pio_init();
size_t utf8stowcs(wchar_t *, char *,size_t);
size_t wcstoutf8s(char *, wchar_t *,size_t);


#endif

