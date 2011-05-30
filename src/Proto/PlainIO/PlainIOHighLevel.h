#ifndef PEASANTFORMER_Proto_PlainIOHighLevel
#define PEASANTFORMER_Proto_PlainIOHighLevel

/// @file

#include "PlainIO.h"

#include <string.h>
#include <assert.h>

/// Converts multibyte array to widechar one.
///
/// \param [out] dest is your destnation string to write to
/// \param [in] source C string to convert from
/// \param [in] length is optional length of source string
/// @return length of resulting string
size_t pioh_mbtowc(wchar_t **dest,const char *source, int length = -1);

/// Converts widechar array to multibyte one.
///
/// \param [out] dest is your destination string; must be freed manualy
/// \param [in] source is your widechar string to convert
/// \param [in] length is optional length of the input string
/// @return length of resulting string
size_t pioh_wctomb(char **dest,const wchar_t *source,int length = -1);

#endif
