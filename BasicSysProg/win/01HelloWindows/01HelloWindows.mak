OutputPath=Debug

all : $(OutputPath)/HelloWindows.obj $(OutputPath)/HelloWindows.exe

$(OutputPath)/HelloWindows.obj : HelloWindows.c
    cl /D UNICODE /D _UNICODE /c HelloWindows.c /Fo:$(OutputPath)/HelloWindows.obj

$(OutputPath)/HelloWindows.exe : HelloWindows.obj
    link $(OutputPath)/HelloWindows.obj user32.lib /OUT:$(OutputPath)/HelloWindows.exe

clean :
    del "$(OutputPath)\HelloWindows.obj" "$(OutputPath)\HelloWindows.exe"