// 预编译头文件,不常变化的系统头文件集中放在这里
#pragma once

#include <assert.h>
#include <string>

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>

// $(Configuration)
#ifdef _DEBUG
	#include "..\\x64\\Debug\\XWndLib.h"
	#pragma comment(lib, "..\\x64\\Debug\\XWndLib.lib")
#else
	#include "..\\x64\\Release\\XWndLib.h"
	#pragma comment(lib, "..\\x64\\Release\\XWndLib.lib")
#endif // _DEBUG