#include <stdio.h>
#include "x.h"


CBase2::CBase2()
{
	a = 0x61;
	b = 0x62;
	x = 0x61;
	c = 0x63;
}


void CBase2::Output()
{
	wprintf_s(L"virtual a = %c | b = %d\n", a, b);
}

///////////////////////////////////////

CSon2::CSon2()
{
	d = 0x64;
}

void CSon2::Output()
{
	wprintf_s(L"virtual d = %c\n", d);
}