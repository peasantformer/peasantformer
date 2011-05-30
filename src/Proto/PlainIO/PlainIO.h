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
///
/// \param [out] dest is your destination widechar buffer
/// \param [in] source is your C char source buffer
/// \param [in] length is required length of the source buffer
/// @return number of converted chars
size_t pio_mbstowcs(wchar_t *dest,const char *source,size_t length);

/// Convert widechar string to system's multibyte string
///
/// \param [out] dest is your destination char buffer
/// \param [in] source is you widechar buffer
/// \param [in] length is required length of widechat buffer
/// @return number of converted bytes
size_t pio_wcstombs(char *dest,const wchar_t *source,size_t length);


#endif

