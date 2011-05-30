#ifndef PEASANTFORMER_Proto_PlainIOHighLevel
#define PEASANTFORMER_Proto_PlainIOHighLevel

/// @file

#include "PlainIO.h"

#include <string.h>
#include <assert.h>

/// Converts multibyte array to widechar one
/// @return length of resulting string
size_t pioh_mbtowc(
	wchar_t **dest,     ///< [out] your destination, must be freed manualy
	const char *source, ///< [in] source C string to convert from
	int length = -1     ///< [in] maximum length of this string
);

/// Converts widechar array to multibyte one
/// @return length of resulting string
size_t pioh_wctomb(
	char **dest,           ///< [out] your destination, must be freed manualy
	const wchar_t *source, ///< [in] source widechar string to convert from
	int length = -1        ///< [in] maximum length of this string
);

#endif
