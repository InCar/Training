#include "stdafx.h"
#include "AdvCalc.h"
#include <intrin.h>

AdvCalc::AdvCalc()
{
	// 分配内存
	m_pV = (float*)_aligned_malloc(sizeof(float) * c_len, 64);
	// 随机产生数据
	for (int i = 0;i < c_len;i++) {
		unsigned int val;
		while (!_rdrand32_step(&val));
		m_pV[i] = 1.0f * val / 0xffffffff;
	}
}

AdvCalc::~AdvCalc()
{
	_aligned_free(m_pV);
	m_pV = NULL;
}

// SIMD支持类型
BOOL AdvCalc::IsSupport(SIMD level)
{
	// https://en.wikipedia.org/wiki/CPUID

	int cpuinfo[4], cpuinfo7[4];
	__cpuid(cpuinfo, 0x01);
	__cpuid(cpuinfo7, 0x07);
	int ebx = cpuinfo7[1];
	int ecx = cpuinfo[2];
	int edx = cpuinfo[3];

	BOOL bSupport = FALSE;
	switch (level)
	{
	case NA:
		bSupport = TRUE;
		break;
	case MMX:
		bSupport = edx & 0x00800000;
		break;
	case SSE:
		bSupport = edx & 0x02000000;
		break;
	case AVX:
		bSupport = ecx & 0x10000000;
		break;
	case FMA:
		bSupport = ecx & 0x00001000;
		break;
	case AVX512:
		bSupport = ebx & 0x04000000;
		break;
	default:
		break;
	}

	return bSupport;
}

// 基本计算
float AdvCalc::calcBasic()
{
	float fSum;
	for (int j = 0;j < c_loop;j++) {
		fSum = 0.0f;
		for (int i = 0;i < c_len;i++) {
			float fX = m_fA * m_pV[i] + m_fB;
			fSum += fX;
		}
	}
	return fSum;
}

// SSE
float AdvCalc::calcSSE()
{
	float fSum;
	for (int j = 0;j < c_loop;j++) {
		__m128 a = _mm_set_ps1(m_fA);
		__m128 b = _mm_set_ps1(m_fB);
		__m128* pV = (__m128*)m_pV;
		__m128 sum = _mm_set_ps1(0.0f);

		for (int i = 0;i < c_len / 4;i++) {
			__m128 x = _mm_mul_ps(a, pV[i]);
			x = _mm_add_ps(x, b);
			sum = _mm_add_ps(sum, x);
		}

		float* pSum = (float*)_aligned_malloc(sizeof(float) * 4, 64);
		_mm_store_ps(pSum, sum);

		fSum = 0.0f;
		for (int i = 0;i < 4;i++) {
			fSum += pSum[i];
		}
		_aligned_free(pSum);
	}

	return fSum;
}

// AVX
float AdvCalc::calcAVX()
{
	float fSum;
	for (int j = 0;j < c_loop;j++) {
		__m256 a = _mm256_set1_ps(m_fA);
		__m256 b = _mm256_set1_ps(m_fB);
		__m256* pV = (__m256*)m_pV;
		__m256 sum = _mm256_set1_ps(0.0f);

		for (int i = 0;i < c_len / 8;i++) {
			__m256 x = _mm256_mul_ps(a, pV[i]);
			x = _mm256_add_ps(x, b);
			sum = _mm256_add_ps(sum, x);
		}

		sum = _mm256_hadd_ps(sum, sum);
		sum = _mm256_hadd_ps(sum, sum);
		sum = _mm256_hadd_ps(sum, sum);

		float* pSum = (float*)_aligned_malloc(sizeof(float) * 8, 64);
		_mm256_store_ps(pSum, sum);
		fSum = *pSum;
		_aligned_free(pSum);
	}
	return fSum;
}

// FMA
float AdvCalc::calcFMA()
{
	float fSum;
	for (int j = 0;j < c_loop;j++) {
		__m256 a = _mm256_set1_ps(m_fA);
		__m256 b = _mm256_set1_ps(m_fB);
		__m256* pV = (__m256*)m_pV;
		__m256 sum = _mm256_set1_ps(0.0f);

		for (int i = 0;i < c_len / 8;i++) {
			__m256 x = _mm256_fmadd_ps(pV[i], a, b);
			sum = _mm256_add_ps(sum, x);
		}

		sum = _mm256_hadd_ps(sum, sum);
		sum = _mm256_hadd_ps(sum, sum);
		sum = _mm256_hadd_ps(sum, sum);

		float* pSum = (float*)_aligned_malloc(sizeof(float) * 8, 64);
		_mm256_store_ps(pSum, sum);
		fSum = *pSum;
		_aligned_free(pSum);
	}
	return fSum;
}

// AVX512
float AdvCalc::calcAVX512()
{
	float fSum;
	for (int j = 0;j < c_loop;j++) {
		__m512 a = _mm512_set1_ps(m_fA);
		__m512 b = _mm512_set1_ps(m_fB);
		__m512* pV = (__m512*)m_pV;
		__m512 sum = _mm512_set1_ps(0.0f);

		for (int i = 0;i < c_len / 16;i++) {
			__m512 x = _mm512_fmadd_ps(pV[i], a, b);
			sum = _mm512_add_ps(sum, x);
		}

		fSum = _mm512_reduce_add_ps(sum);
	}
	return fSum;
}