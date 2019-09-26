#include <stdio.h>
#include "x.h"

CBase::CBase()
{
		a = 0x61;
		b = 0x62;
		x = 0x61;
		c = 0x63;
}


void CBase::Output()
{
	wprintf_s(L"a = %c | b = %d\n", a, b);
}

///////////////////////////////////

CSon::CSon()
{
	d = 0x64;
}

void CSon::Output()
{
	wprintf_s(L"d = %c\n", d);
}