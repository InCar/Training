// 预编译头文件,不常变化的系统头文件集中放在这里
#pragma once

#include <assert.h>
#include <string>

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>

#include "..\\XWndLib\\XWndLib.h"
// $(Configuration)
#ifdef _DEBUG
	#pragma comment(lib, "..\\x64\\Debug\\XWndLib.lib")
#else
	#pragma comment(lib, "..\\x64\\Release\\XWndLib.lib")
#endif // _DEBUG