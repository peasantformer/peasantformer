#ifndef PEASANTFORMER_Proto_PlainIO
#define PEASANTFORMER_Proto_PlainIO

/// @file

#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#ifdef _WIN32

#include <windows.h>

#endif

/// Initialize terminal for unicode Input/Output.
void pio_init();

/// Convert system's multibyte string to widechar string
size_t utf8stowcs(
	wchar_t *dest, ///< [out] destination widechar_t buffer
	const char *source,///< [in] source char* buffer
	size_t length ///< [in] length of char* buffer
);
/// Convert widechar string to system's multibyte string
size_t wcstoutf8s(
	char *dest, ///< [out] destination char* buffer
	const wchar_t *source, ///< [in] source widechar_t buffer
	size_t length ///< [in] length of widechar_t buffer
);


#endif

