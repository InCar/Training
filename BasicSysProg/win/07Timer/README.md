# Timer
定时器

![timer](http://ic-media.oss-cn-hangzhou.aliyuncs.com/2016%2F04%2Ftimer.png)

## windowsx.h
```C
switch (uMsg) {
    HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
    HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
    HANDLE_MSG(hWnd, WM_TIMER, OnTimer);
    HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
    HANDLE_MSG(hWnd, WM_KEYDOWN, OnKeyDown);
default:
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
```

## Timer
+ SetTimer
+ KillTimer