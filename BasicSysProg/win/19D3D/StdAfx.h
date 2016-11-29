#pragma once

#include <Windows.h>
#include <windowsx.h>
#include <Dwmapi.h>
#include <d3d11.h>
#include <dxgi1_3.h>
#include <directxcolors.h>
#include <wrl.h>
#include <string>

#pragma comment(lib, "Dwmapi.lib")
#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "DXGI.lib")

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")