# 02 Message Loop

## Standard Steps
1. Register window class
2. Create window
3. Run message loop

### 1. Register window class
Registers a window class for subsequent use in calls to the CreateWindow.
```C
ATOM WINAPI RegisterClass(
  _In_ const WNDCLASS *lpWndClass
);
```

```C
typedef struct tagWNDCLASS {
  UINT      style;
  WNDPROC   lpfnWndProc;
  int       cbClsExtra;
  int       cbWndExtra;
  HINSTANCE hInstance;
  HICON     hIcon;
  HCURSOR   hCursor;
  HBRUSH    hbrBackground;
  LPCTSTR   lpszMenuName;
  LPCTSTR   lpszClassName;
} WNDCLASS, *PWNDCLASS;
```

### 2. Create window
Create the main window.
```C
HWND WINAPI CreateWindow(
  _In_opt_ LPCTSTR   lpClassName,
  _In_opt_ LPCTSTR   lpWindowName,
  _In_     DWORD     dwStyle,
  _In_     int       x,
  _In_     int       y,
  _In_     int       nWidth,
  _In_     int       nHeight,
  _In_opt_ HWND      hWndParent,
  _In_opt_ HMENU     hMenu,
  _In_opt_ HINSTANCE hInstance,
  _In_opt_ LPVOID    lpParam
);
```

### 3. Run message loop
Start the message loop.
```C
while (GetMessage(&msg, NULL, 0, 0)) {
	TranslateMessage(&msg);
	DispatchMessage(&msg);
}
```

## Window Process
An application-defined function that processes messages sent to a window.
```C
LRESULT CALLBACK WindowProc(
  _In_ HWND   hwnd,
  _In_ UINT   uMsg,
  _In_ WPARAM wParam,
  _In_ LPARAM lParam
);
```

### Important window messages
* WM_CREATE
* WM_DESTROY
* WM_PAINT

### Let `DefWindowProc` do the rest
```C
LRESULT WINAPI DefWindowProc(
  _In_ HWND   hWnd,
  _In_ UINT   Msg,
  _In_ WPARAM wParam,
  _In_ LPARAM lParam
);
```