OutputPath=Debug

all : $(OutputPath)/HelloWindows.obj $(OutputPath)/HelloWindows.exe

$(OutputPath)/HelloWindows.obj : HelloWindows.c
    - if NOT EXIST $(OutputPath) mkdir $(OutputPath)
	- cl /D UNICODE /D _UNICODE /c /Fo:$(OutputPath)/HelloWindows.obj HelloWindows.c

$(OutputPath)/HelloWindows.exe : $(OutputPath)/HelloWindows.obj
    - link $(OutputPath)/HelloWindows.obj user32.lib /OUT:$(OutputPath)/HelloWindows.exe

clean :
    - del "$(OutputPath)\HelloWindows.obj" "$(OutputPath)\HelloWindows.exe"