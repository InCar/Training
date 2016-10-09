#include "stdafx.h"
#include "16SD.h"

DLL_EXPORT int CallDynamicLib(wchar_t * pwszBuf, int count)
{
    static const wchar_t *wszHello = L"Hello Dynamic Library";

    int cch = (int)wcslen(wszHello);
    HRESULT hr = StringCchCopy(pwszBuf, count, wszHello);
    if (SUCCEEDED(hr))
        return cch < count ? cch : count;
    else
        return -1;
}
