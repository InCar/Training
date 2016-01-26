# 01 Hello .Net
![dotNet](http://i.microsoft.com/net/images/chrome/net-logo.jpg)
+ Microsoft .Net Framework - Only in windows
+ Mono Framework - Only in Linux
+ Core CLR & Framework - Windows, Linux, MacOS

## Prerequisite
[.Net SDK and VS2015 Community](http://getdotnet.azurewebsites.net/target-dotnet-platforms.html)

## Build
You can build with anyone of the 2 ways listed below.
### 1. Build step by step
```SHELL
csc HelloDotNet.cs /t:module /r:WPF/PresentationFramework.dll
al HelloDotNet.netmodule /t:win /main:HelloDotNet.App.Main /out:HelloDotNet.exe
```

### 2. Build in one step
```SHELL
csc HelloDotNet.cs /t:winexe /r:WPF/PresentationFramework.dll
```

## Run
```SHELL
HelloDotNet.exe
```

## 64bits vs. 32bits
```SHELL
corflags HelloDotNet.exe /32BITPREF+
# or
corflags HelloDotNet.exe /32BITPREF-
```

## dnvm & dnx
`dnvm` is the .NET Version Manager. https://github.com/aspnet/dnvm/