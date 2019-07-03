#pragma once

#ifdef _USRDLL
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT  __declspec(dllimport)
#endif // _USRDLL


DLL_EXPORT int CallDynamicLib(wchar_t *pwszBuf, int count);