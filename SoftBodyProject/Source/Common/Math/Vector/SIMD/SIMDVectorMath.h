#pragma once

#include "SIMDVector.h"

namespace SIMDVectorMath
{
	// ---SIMDVectorFloatの演算---
	// 加算
	SIMDVectorFloat Add(const SIMDVectorFloat& _vec1, const SIMDVectorFloat& _vec2);
	// 減算
	SIMDVectorFloat Sub(const SIMDVectorFloat& _vec1, const SIMDVectorFloat& _vec2);
	// 乗算
	SIMDVectorFloat Mul(const SIMDVectorFloat& _vec1, const SIMDVectorFloat& _vec2);
	// 割り算
	SIMDVectorFloat Div(const SIMDVectorFloat& _vec1, const SIMDVectorFloat& _vec2);

	// スカラー倍
	SIMDVectorFloat MulScalar(const SIMDVectorFloat& _vec, float _value);
	// スカラー割
	SIMDVectorFloat DivScalar(const SIMDVectorFloat& _vec, float _value);

	// 内積(Vector2)
	float Dot2(const SIMDVectorFloat& _vec1, const SIMDVectorFloat& _vec2);
	// 内積(Vector3)
	float Dot3(const SIMDVectorFloat& _vec1, const SIMDVectorFloat& _vec2);
	// 内積(Vector4)
	float Dot4(const SIMDVectorFloat& _vec1, const SIMDVectorFloat& _vec2);

	// 外積(Vector3)
	SIMDVectorFloat Cross3(const SIMDVectorFloat& _vec1, const SIMDVectorFloat& _vec2);

	// 長さ(Vector2)
	float Length2(const SIMDVectorFloat& _vec);
	// 長さ(Vector3)
	float Length3(const SIMDVectorFloat& _vec);
	// 正規化
	SIMDVectorFloat Normalize(const SIMDVectorFloat& _vec);

	// 平方根
	SIMDVectorFloat Sqrt(const SIMDVectorFloat& _vec);
	// 逆平方根
	SIMDVectorFloat RSqrt(const SIMDVectorFloat& _vec);

	// 逆数
	SIMDVectorFloat Reciprocal(const SIMDVectorFloat& _vec);

	// 反転
	SIMDVectorFloat Negate(const SIMDVectorFloat& _vec);

	// 等価
	bool Equals(const SIMDVectorFloat& _a, const SIMDVectorFloat& _b);

	// and演算
	SIMDVectorFloat And(const SIMDVectorFloat& _vec1, const SIMDVectorFloat& _vec2);

	// 横足し算
	float HorizontalAdd(const SIMDVectorFloat& _vec);

	// ---SIMDVectorIntの演算---
	// 加算
	SIMDVectorInt Add(const SIMDVectorInt& _vec1, const SIMDVectorInt& _vec2);
	// 減算
	SIMDVectorInt Sub(const SIMDVectorInt& _vec1, const SIMDVectorInt& _vec2);
	// 乗算
	SIMDVectorInt Mul(const SIMDVectorInt& _vec1, const SIMDVectorInt& _vec2);

	// スカラー倍
	SIMDVectorInt MulScalar(const SIMDVectorInt& _vec, int _value);

	// 等価
	bool Equals(const SIMDVectorInt& a, const SIMDVectorInt& b);
};