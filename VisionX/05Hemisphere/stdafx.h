#pragma once

#include <Windows.h>
#include <windowsx.h>
#include <Dwmapi.h>
#include <gdiplus.h>

#include <wrl.h>
#include <d3d11.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <directxcolors.h>

#include <string>
#include <memory>
#include <thread>

#pragma comment(lib, "Dwmapi.lib")
#pragma comment(lib, "D3D11.lib")
#pragma comment(lib, "DXGI.lib")
#pragma comment(lib, "gdiplus.lib")

#include <opencv2/opencv.hpp>

#ifdef _DEBUG
	#pragma comment(lib, "opencv_world440d.lib")
#else
	#pragma comment(lib, "opencv_world440.lib")
#endif //  DEBUG