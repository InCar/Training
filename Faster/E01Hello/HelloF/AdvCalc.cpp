#include "stdafx.h"
#include "AdvCalc.h"
#include <intrin.h>

AdvCalc::AdvCalc()
{
	// 分配内存
	m_pV = (float*)_aligned_malloc(sizeof(float) * c_len, 32);
	// 随机产生数据
	for (int i = 0;i < c_len;i++) {
		m_pV[i] = 1.0f * rand() / RAND_MAX;
	}
}

AdvCalc::~AdvCalc()
{
	_aligned_free(m_pV);
	m_pV = NULL;
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

		float* pSum = (float*)_aligned_malloc(sizeof(float) * 4, 32);
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

		float* pSum = (float*)_aligned_malloc(sizeof(float) * 8, 32);
		_mm256_store_ps(pSum, sum);
		fSum = *pSum;
		_aligned_free(pSum);
	}
	return fSum;
}