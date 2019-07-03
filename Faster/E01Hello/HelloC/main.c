#include <stdio.h>
#include <locale.h>

int wmain(int argc, wchar_t* argv[], wchar_t* envp[])
{
	for (int i = 0;i < argc;i++) {
		wprintf_s(L"%d: %s\n", i, argv[i]);
	}
	return 0;
}