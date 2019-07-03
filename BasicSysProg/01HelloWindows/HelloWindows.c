#include <Windows.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrev, PWSTR pCmdLine, int nCmdShow)
{
	MessageBox(NULL, L"Hello Windows!\r\nThis is my first Windows program!", L"01 Hello Windows", MB_OK);
	return 0;
}