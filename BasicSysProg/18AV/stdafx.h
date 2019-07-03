#pragma once

#include <Windows.h>
#include <windowsx.h>
#include <CommCtrl.h>
#include <Shobjidl.h>
#include <Objbase.h>
#include <Dwmapi.h>
#include <Mmdeviceapi.h>
#include <Audioclient.h>
#include <mfapi.h>
#include <mfidl.h>
#include <Mfreadwrite.h>
#include <Strsafe.h>
#include <malloc.h>
#include <string>

#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "Dwmapi.lib")
#pragma comment(lib, "Mf.lib")
#pragma comment(lib, "Mfplat.lib")
#pragma comment(lib, "mfuuid.lib")
#pragma comment(lib, "Mfreadwrite.lib")

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")