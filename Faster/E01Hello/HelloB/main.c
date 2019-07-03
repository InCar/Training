#include <stdio.h>

int main(int argc, char* argv[], char* envp[])
{
	for (int i = 0;i < argc;i++) {
		printf_s("%d: %s\n", i, argv[i]);
	}

	printf_s("-----\n");

	char** ppsz = envp;
	while (*ppsz != NULL) {
		printf_s("\t%s\n", *ppsz);
		ppsz++;
	}
	
	return 0;
}