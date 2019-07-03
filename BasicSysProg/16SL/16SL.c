#include "stdafx.h"
#include "16SL.h"

DLL_EXPORT int CallDynamicLib(wchar_t * pwszBuf, int count)
{
    static int s_count = 0;
    static const wchar_t *wszHello = L"Hello Dynamic Load Library %4d";

    int cch = (int)wcslen(wszHello);
    HRESULT hr = StringCchPrintf(pwszBuf, count, wszHello, s_count++);
    if (SUCCEEDED(hr))
        return cch < count ? cch : count;
    else
        return -1;
}
