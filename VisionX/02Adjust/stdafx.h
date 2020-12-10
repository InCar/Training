#pragma once

#include <Windows.h>
#include <windowsx.h>
#include <Dwmapi.h>

#include <string>
#include <memory>
#include <thread>

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")

#include <opencv2/opencv.hpp>

#pragma comment(lib, "Dwmapi.lib")
#ifdef _DEBUG
	#pragma comment(lib, "opencv_world440d.lib")
#else
	#pragma comment(lib, "opencv_world440.lib")
#endif //  DEBUG