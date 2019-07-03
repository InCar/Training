# 01 Hello Windows

![VS2015](https://assets.windowsphone.com/db658987-b7ca-43aa-885c-fd4426fb6962/Downloads-VS_InvariantCulture_Default.png)

## Prerequisite
[Windows SDK](https://dev.windows.com/en-us/downloads)

## Build
You can build with anyone of the 3 ways listed below.

### A. build step by step
```SHELL
cl /c /D UNICODE /D _UNICODE HelloWindows.c
link HelloWindows.obj user32.lib
```

### B. build in one step
```SHELL
cl HelloWindows.c /D UNICODE /D _UNICODE /link user32.lib
```

### C. build with nmake
```SHELL
nmake 01HelloWindows.mak
```

## Run
```SHELL
HelloWindows.exe
```

## Reference
+ [WinMain](https://msdn.microsoft.com/en-us/library/windows/desktop/ms633559.aspx)
+ [MessageBox](https://msdn.microsoft.com/en-us/library/windows/desktop/ms645505.aspx)
+ [cl.exe](https://msdn.microsoft.com/en-us/library/19z1t1wy.aspx)
+ [link.exe](https://msdn.microsoft.com/en-us/library/y0zzbyt4.aspx)
+ [nmake.exe](https://msdn.microsoft.com/en-us/library/dd9y37ha.aspx)
+ [depends.exe](http://dependencywalker.com/)

## Outline
+ History
    - Turing & Von Neumann
    - Fairchild => Intel, AMD => Microsoft, Apple, Oracle(Sun), Google
    - ASM => C => C++,Objective-C => Java(JVM), C#(CLR) | Javascript(Browser|nodejs)
    - MS-DOS => Win3.1 => Windows9X => WindowsNT
+ Hello Windows
+ Dependencies
+ Image vs. Process
+ Memory