#include "stdafx.h"
#include "resource.h"
#include "MainWnd.h"

MainWnd::MainWnd()
{
}

MainWnd::~MainWnd()
{
}

ATOM MainWnd::Register()
{
	// 主窗口类型注册,隐藏了基类方法
	WNDCLASS wcls;
	ZeroMemory(&wcls, sizeof(WNDCLASS));
	wcls.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcls.lpfnWndProc = XWnd::WindowProc;
	wcls.hInstance = m_hInst;
	wcls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcls.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcls.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_ICONAPP)); // 图标
	wcls.lpszClassName = m_wstrClsName.c_str();

	return RegisterClass(&wcls);
}

BOOL MainWnd::Create()
{
	// 窗口尺寸和位置
	int cxScreen = GetSystemMetrics(SM_CXSCREEN);
	int cyScreen = GetSystemMetrics(SM_CYSCREEN);
	int width = cxScreen > 800 ? 800 : cxScreen;
	int height = cyScreen > 600 ? 600 : cyScreen;

	WNDCLASS cls;
	if (!GetClassInfo(m_hInst, m_wstrClsName.c_str(), &cls)) Register();

	// 创建窗口
	wstring wstrTitle(L"Move");
	m_hwnd = CreateWindow(m_wstrClsName.c_str(), wstrTitle.c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		(cxScreen - width) / 2, (cyScreen - height) / 2, width, height,
		NULL, NULL, m_hInst, this);
	if (!m_hwnd) {
		MessageBox(NULL, L"Create window failed", L"Error", MB_OK);
		return FALSE;
	}

	// 第一次刷新窗口
	UpdateWindow(m_hwnd);

	return TRUE;
}

LRESULT MainWnd::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 消息处理
	switch (uMsg) {
		HANDLE_MSG(hWnd, WM_KEYDOWN, OnKeyDown);
		break;
	case WM_CAR_MOVING:
		return OnCarMoving(hWnd, uMsg, wParam, lParam);
	default:
		return XWnd::WndProc(hWnd, uMsg, wParam, lParam);
	}
}

BOOL MainWnd::OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	__super::OnCreate(hwnd, lpCreateStruct);

	m_hDotPen = CreatePen(PS_DOT, 0, RGB(0xff, 0x00, 0x00));
	m_hbrCar = CreateSolidBrush(RGB(0x33, 0x33, 0xcc));
	return TRUE;
}

void MainWnd::OnDestroy(HWND hwnd)
{
	DeletePen(m_hDotPen);
	DeleteBrush(m_hbrCar);
	__super::OnDestroy(hwnd);
}

void MainWnd::OnPaint(HWND hwnd)
{
	// 获取主显示器高度(以像素为单位)
	int pixelsInHeight = GetSystemMetrics(SM_CYSCREEN);
	int pixels = static_cast<int>(pixelsInHeight * 0.9f);
	// 获取窗口尺寸(以显示器设备像素为单位)
	RECT rc;
	GetClientRect(hwnd, &rc);

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);

	// 设定坐标系统
	SetMapMode(hdc, MM_ISOTROPIC);
	SetWindowExtEx(hdc, 100000, 100000, NULL); // 标准世界有多大
	SetViewportExtEx(hdc, pixels, -pixels, NULL); // 用显示器设备上的多少像素展示标准世界
	SetViewportOrgEx(hdc, rc.right / 2, rc.bottom / 2, NULL); // 标准世界的(0,0)相对窗口左上角在什么位置(以显示器设备像素为单位)

	// 标准世界边界
	Rectangle(hdc, -50000, -50000, 50000, 50000);

	// 坐标轴
	SelectPen(hdc, m_hDotPen);
	MoveToEx(hdc, -50000, 0, NULL);
	LineTo(hdc, +50000, 0);
	MoveToEx(hdc, 0, -50000, NULL);
	LineTo(hdc, 0, +50000);

	// 车辆
	RECT rcCar;
	FLOAT2 pos = m_car.GetPos();
	FLOAT2 size = m_car.GetSize();
	rcCar.left = static_cast<int>(1000.0f * (pos.x - size.x / 2.0f));
	rcCar.right = static_cast<int>(1000.0f * (pos.x + size.x / 2.0f));
	rcCar.top = static_cast<int>(1000.0f * (pos.y + size.y / 2.0f));
	rcCar.bottom = static_cast<int>(1000.0f * (pos.y - size.y / 2.0f));
	FillRect(hdc, &rcCar, m_hbrCar);

	EndPaint(hwnd, &ps);

	// 继续移动
	PostMessage(hwnd, WM_CAR_MOVING, 0, 0);
}

void MainWnd::OnKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
{
	switch (vk)
	{
	case VK_UP:
	case VK_DOWN:
	case VK_LEFT:
	case VK_RIGHT:
		PostMessage(hwnd, WM_CAR_MOVING, 0, 0);
		break;
	default:
		break;
	}
}

LRESULT MainWnd::OnCarMoving(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 速率
	float fSpeed = 0.01f;
	// 方向
	int iX = (GetKeyState(VK_RIGHT) >> 8 & 0x01) - (GetKeyState(VK_LEFT) >> 8 & 0x01);
	int iY = (GetKeyState(VK_UP) >> 8 & 0x01) - (GetKeyState(VK_DOWN) >> 8 & 0x01);

	// 计算位移
	float fDeltaX = fSpeed * iX;
	float fDeltaY = fSpeed * iY;

	// 如果需要移动车辆
	if (iX != 0 || iY != 0) {
		m_car.Move(fDeltaX, fDeltaY);
		InvalidateRect(hwnd, NULL, TRUE);
	}

	return 0L;
}
