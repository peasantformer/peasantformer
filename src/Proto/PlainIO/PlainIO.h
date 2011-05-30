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
/// @return number of converted chars
size_t pio_mbstowcs(
	wchar_t *dest, ///< [out] destination widechar_t buffer
	const char *source,///< [in] source char* buffer
	size_t length ///< [in] length of char* buffer
);
/// Convert widechar string to system's multibyte string
/// @return number of converted bytes
size_t pio_wcstombs(
	char *dest, ///< [out] destination char* buffer
	const wchar_t *source, ///< [in] source widechar_t buffer
	size_t length ///< [in] length of widechar_t buffer
);


#endif

