#pragma once
enum SIMD
{
	NA = 0,
	MMX,
	SSE,
	AVX,
	FMA,
	AVX512
};

class AdvCalc
{
public:
	AdvCalc();
	virtual ~AdvCalc();

	// SIMD支持类型
	BOOL IsSupport(SIMD level);

	// 基本计算
	float calcBasic();
	// MMX
	float calcSSE();
	// AVX
	float calcAVX();
	// FMA
	float calcFMA();
	// AVX512
	float calcAVX512();
private:
	// stress
	const int c_loop = 10000;
	// count
	const int c_len = 8000;
	// float a
	const float m_fA = 3.6f;
	// float b
	const float m_fB = 120.86f;
	// float values
	float* m_pV = NULL;
};
