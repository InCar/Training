#pragma once
class AdvCalc
{
public:
	AdvCalc();
	virtual ~AdvCalc();

	// »ù±¾¼ÆËã
	float calcBasic();

	// MMX
	float calcSSE();

	// AVX
	float calcAVX();
private:
	// stress
	const int c_loop = 1000;
	// count
	const int c_len = 8000;
	// float a
	const float m_fA = 3.6f;
	// float b
	const float m_fB = 120.86f;
	// float values
	float* m_pV = NULL;
};

