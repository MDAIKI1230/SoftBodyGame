#pragma once

#include "../SIMD/SIMDVector.h"

struct Vector4
{
public:
	union
	{
		// アクセス用数値
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};

		// SIMD計算用
		SIMDVectorFloat simd;
	};
public:
	// コンストラクタ
	Vector4() :
		simd{ 0.0f,0.0f,0.0f,0.0f }
	{
	}

	/// <param name="simd">SIMD</param>
	Vector4(SIMDVectorFloat simd) :
		simd(simd)
	{
	}

	/// <param name="_x">X</param>
	/// <param name="_y">Y</param>
	Vector4(float _x, float _y, float _z, float _w) :
		simd(_x, _y, _z, _w)
	{
	}

	// 型変換
	operator SIMDVectorFloat() const
	{
		return simd;
	}

	// 加算
	Vector4 operator+(const Vector4& _other)const;
	// 減算
	Vector4 operator-(const Vector4& _other)const;
	// 乗算
	Vector4 operator*(float _value)const;
	/// <summary>
	/// 徐算(整数の場合static_castで整数になります)
	/// </summary>
	/// <returns>0徐算の場合は、そのままの値を返します</returns>
	Vector4 operator/(float _value)const;

	// 加算
	Vector4& operator+=(const Vector4& _other);
	// 減算
	Vector4& operator-=(const Vector4& _other);
	// 乗算
	Vector4& operator*=(float _value);
	/// <summary>
	/// 徐算(整数の場合static_castで整数になります)
	/// </summary>
	/// <returns>0徐算の場合は、そのままの値を返します</returns>
	Vector4& operator/=(float _value);

	// 逆ベクトル
	Vector4 operator-()const;

	// 等価
	bool operator==(const Vector4& _other) const;
	// 不等価
	bool operator!=(const Vector4& _other) const;

	// ノルム
	float Norm() const;
	// ノルム
	static float Norm(const Vector4& _vec);

	// 正規化(値の変化有)(実数のみ)
	Vector4& Normalize();
	// 正規化(値の変化有)(実数のみ)
	static Vector4& Normalize(Vector4& _vec);

	// 正規化(値の変化無)(実数のみ)
	Vector4 Normalized() const;
	// 正規化(値の変化無)(実数のみ)
	static Vector4 Normalized(const Vector4& _vec);

	// 内積
	static float Dot(const Vector4& _vec1, const Vector4& _vec2);
};
