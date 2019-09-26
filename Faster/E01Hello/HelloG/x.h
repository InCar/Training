#pragma once
class CBase
{
public:
	CBase();
	void Output();
private:
	char a;
	char b;
	int x;
	char c;
};

class CSon : public CBase
{
public:
	CSon();
	void Output();
private:
	char d;
};


class CBase2
{
public:
	CBase2();
	virtual void Output();
private:
	char a;
	char b;
	int x;
	char c;
};

class CSon2 : public CBase2
{
public:
	CSon2();
	void Output();
private:
	char d;
};