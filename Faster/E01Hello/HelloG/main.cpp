#include <wchar.h>
#include <stdlib.h>

// function declare
void HowToRefVar();
void HowToInterpretation();
void HowToInterpretation2();

typedef void(*HowTo)();

int wmain()
{
	// 变量的指针 vs 变量的引用
	HowToRefVar();

	// 如何解释
	HowToInterpretation();
	HowToInterpretation2();

	// 函数由它的内存首地址来表示
	HowTo fnHowTo;
	fnHowTo = HowToInterpretation;
	fnHowTo();

	// 函数的首地址也可以存放在一个变量里
	HowTo* pFn = &fnHowTo;
	(*pFn)();
	*pFn = HowToRefVar;
	(*pFn)();

	return 0;
}

// function implementation
void HowToRefVar()
{
	// 变量
	int x = 0x61;

	// 变量的指针
	int* px = &x;

	// 变量的引用
	int& rx = x;

	wchar_t buf[128];
	swprintf_s(buf, L"%x : %x : %x", x, *px, rx);			// 以16进制显示数值
	swprintf_s(buf, L"%d : %d : %d", x, *px, rx);			// 以10进制显示数值
	swprintf_s(buf, L"%p : %p : %p", &x, px, &rx);			// 显示变量x的内存地址
	swprintf_s(buf, L"%p : %p : %p", &px, buf, &buf[0]);	// px的内存地址,buf的首地址,buf第一个元素的地址
}

void HowToInterpretation()
{
	// 数据本质上只是内存,怎么解释数据由你自己决定
	char data[8];

	for (int i = 0; i < 8; i++)
	{
		data[i] = 0x61 + i;
	}

	wchar_t buf[64];
	swprintf_s(buf, L"%d %d %d %d", data[0], data[1], data[2], data[3]);
	swprintf_s(buf, L"%x %x %x %x", data[0], data[1], data[2], data[3]);
	swprintf_s(buf, L"0x%x 0x%x 0x%x 0x%x", data[0], data[1], data[2], data[3]);
	swprintf_s(buf, L"%c %c %c %c", data[0], data[1], data[2], data[3]);

	swprintf_s(buf, L"0x%x 0x%x 0x%x 0x%x", *((short*)& data[0]), *((short*)& data[2]), *((short*)& data[4]), *((short*)& data[6]));
	swprintf_s(buf, L"%d %d %d %d", *((short*)&data[0]), *((short*)& data[2]), *((short*)& data[4]), *((short*)& data[6]));
}

struct S4
{
	char s0;
	char s1;
	char s2;
	char s3;
};

struct T1
{
	int t0;
};

void HowToInterpretation2()
{
	wchar_t buf[64];

	void* p = malloc(5);

	char* p1 = (char*)p;
	*p1 = 'a';
	*(p1 + 1) = 'b';
	*(p1 + 2) = 'c';
	p1[3] = 'd';
	p1[4] = 0;
	swprintf_s(buf, L"%d %d %d %d", p1[0], p1[1], p1[2], *(p1+3));

	short* p2 = (short*)p;
	p2[0] = 0x4241;
	p2[1] = 0x4443;
	swprintf_s(buf, L"%d %d %d %d", p1[0], p1[1], p1[2], p1[3]);
	swprintf_s(buf, L"%S", (char*)p2);

	S4* pS4 = (S4*)p;
	S4& rS4 = *pS4;
	swprintf_s(buf, L"%d %d %d %d", pS4->s0, pS4->s1, rS4.s2, rS4.s3);

	T1* pT1 = (T1*)p;
	swprintf_s(buf, L"%d", pT1->t0);

	free(p);
}