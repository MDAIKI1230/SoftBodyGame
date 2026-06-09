#include <smmintrin.h>
#include <math.h>
#include "SIMDVectorMath.h"

// 加算
SIMDVectorFloat SIMDVectorMath::Add(const SIMDVectorFloat& _vec1, const SIMDVectorFloat& _vec2)
{
	return _mm_add_ps(_vec1, _vec2);
}

// 減算
SIMDVectorFloat SIMDVectorMath::Sub(const SIMDVectorFloat& _vec1, const SIMDVectorFloat& _vec2)
{
	return _mm_sub_ps(_vec1, _vec2);
}

// 乗算
SIMDVectorFloat SIMDVectorMath::Mul(const SIMDVectorFloat& _vec1, const SIMDVectorFloat& _vec2)
{
	return _mm_mul_ps(_vec1, _vec2);
}

// 割り算
SIMDVectorFloat SIMDVectorMath::Div(const SIMDVectorFloat& _vec1, const SIMDVectorFloat& _vec2)
{
	return _mm_div_ps(_vec1, _vec2);
}

// スカラー倍
SIMDVectorFloat SIMDVectorMath::MulScalar(const SIMDVectorFloat& _vec, float _value)
{
	return Mul(_vec, SIMDVectorFloat(_value));
}

// スカラー割
SIMDVectorFloat SIMDVectorMath::DivScalar(const SIMDVectorFloat& _vec, float _value)
{
	return Div(_vec, SIMDVectorFloat(_value));
}

// 内積(Vector2)
float SIMDVectorMath::Dot2(const SIMDVectorFloat& _vec1, const SIMDVectorFloat& _vec2)
{
	return _mm_cvtss_f32(_mm_dp_ps(_vec1, _vec2, 0x31));
}

// 内積(Vector3)
float SIMDVectorMath::Dot3(const SIMDVectorFloat& _vec1, const SIMDVectorFloat& _vec2)
{
	return _mm_cvtss_f32(_mm_dp_ps(_vec1, _vec2, 0xE1));
}

// 内積(Vector4)
float SIMDVectorMath::Dot4(const SIMDVectorFloat& _vec1, const SIMDVectorFloat& _vec2)
{
	return _mm_cvtss_f32(_mm_dp_ps(_vec1, _vec2, 0xFF));
}

// 外積(Vector3)
SIMDVectorFloat SIMDVectorMath::Cross3(const SIMDVectorFloat& _vec1, const SIMDVectorFloat& _vec2)
{
	SIMDVectorFloat yzx{ SIMDVectorFloat::Shuffle<1,2,0,3>(_vec1) };
	SIMDVectorFloat zxy{ SIMDVectorFloat::Shuffle<2,0,1,3>(_vec2) };

	// 左側の掛け算
	SIMDVectorFloat temp1{ Mul(yzx ,zxy) };

	zxy = SIMDVectorFloat::Shuffle<2, 0, 1, 3>(_vec1);
	yzx = SIMDVectorFloat::Shuffle<1, 2, 0, 3>(_vec2);

	// 右の掛け算
	SIMDVectorFloat temp2{ Mul(zxy,yzx) };

	// 引き算
	return Sub(temp1, temp2);
}

// 長さ(Vector2)
float SIMDVectorMath::Length2(const SIMDVectorFloat& _vec)
{
	return sqrtf(Dot2(_vec, _vec));
}

// 長さ(Vector3)
float SIMDVectorMath::Length3(const SIMDVectorFloat& _vec)
{
	return sqrtf(Dot3(_vec, _vec));
}

// 正規化
SIMDVectorFloat SIMDVectorMath::Normalize(const SIMDVectorFloat& _vec)
{
	SIMDVectorFloat rsqrt{ RSqrt(Dot4(_vec,_vec)) };
	
	return Mul(_vec, rsqrt);
}

// 平方根
SIMDVectorFloat SIMDVectorMath::Sqrt(const SIMDVectorFloat& _vec)
{
	return _mm_sqrt_ps(_vec);
}

// 逆平方根
SIMDVectorFloat SIMDVectorMath::RSqrt(const SIMDVectorFloat& _vec)
{
	SIMDVectorFloat rsqrt{ _mm_rsqrt_ps(_vec) };

	SIMDVectorFloat half{ 0.5f };
	SIMDVectorFloat three{ 3.0f };

	// ニュートン法で補正
	return Mul(rsqrt, Mul(Sub(three, Mul(_vec, Mul(rsqrt, rsqrt))), half));
}

// 逆数
SIMDVectorFloat SIMDVectorMath::Reciprocal(const SIMDVectorFloat& _vec)
{
	return _mm_rcp_ps(_vec);
}

// 反転
SIMDVectorFloat SIMDVectorMath::Negate(const SIMDVectorFloat& _vec)
{
	return(Sub(_mm_setzero_ps(), _vec));
}

// 等価
bool SIMDVectorMath::Equals(const SIMDVectorFloat& _a, const SIMDVectorFloat& _b)
{
	SIMDVectorFloat cmp = _mm_cmpeq_ps(_a, _b);

	return _mm_movemask_ps(cmp) == 0b1111;
}

// and演算
SIMDVectorFloat SIMDVectorMath::And(const SIMDVectorFloat& _vec1, const SIMDVectorFloat& _vec2)
{
	return _mm_and_ps(_vec1, _vec2);
}

// 横足し算
float SIMDVectorMath::HorizontalAdd(const SIMDVectorFloat& _vec)
{
	SIMDVectorFloat tmp = _mm_hadd_ps(_vec, _vec);
	tmp = _mm_hadd_ps(tmp, tmp);

	return _mm_cvtss_f32(tmp);
}

// 加算
SIMDVectorInt SIMDVectorMath::Add(const SIMDVectorInt& _vec1, const SIMDVectorInt& _vec2)
{
	return _mm_add_epi32(_vec1, _vec2);
}

// 減算
SIMDVectorInt SIMDVectorMath::Sub(const SIMDVectorInt& _vec1, const SIMDVectorInt& _vec2)
{
	return _mm_sub_epi32(_vec1, _vec2);
}

// 乗算
SIMDVectorInt SIMDVectorMath::Mul(const SIMDVectorInt& _vec1, const SIMDVectorInt& _vec2)
{
	return _mm_mullo_epi32(_vec1, _vec2);
}

// スカラー倍
SIMDVectorInt SIMDVectorMath::MulScalar(const SIMDVectorInt& _vec, int _value)
{
	return _mm_mullo_epi32(_vec, SIMDVectorInt(_value));
}

// 等価
bool SIMDVectorMath::Equals(const SIMDVectorInt& a, const SIMDVectorInt& b)
{
	__m128i cmp = _mm_cmpeq_epi32(a.value, b.value);

	return _mm_movemask_epi8(cmp) == 0xFFFF;
}