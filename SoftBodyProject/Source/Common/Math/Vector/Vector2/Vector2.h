#pragma once

#include "../SIMD/SIMDVector.h"

struct alignas(16) Vector2
{
public:
	union
	{
		// アクセス用数値
		struct 
		{
			float x;
			float y;
			float pad[2];
		};

		// SIMD計算用
		SIMDVectorFloat simd;
	};

public:
	static const Vector2 ZERO;
	static const Vector2 ONE;
	static const Vector2 UP;
	static const Vector2 RIGHT;

public:
	// コンストラクタ
	Vector2() :
		simd{ 0.0f,0.0f,0.0f,0.0f }
	{
	}

	/// <param name="simd">SIMD</param>
	Vector2(SIMDVectorFloat simd) :
		simd(simd)
	{
	}
	
	/// <param name="_x">X</param>
	/// <param name="_y">Y</param>
	Vector2(float _x, float _y) :
		simd(_x,_y,0.0f,0.0f)
	{}

	// 型変換
	operator SIMDVectorFloat() const
	{
		return simd;
	}

	// 加算
	Vector2 operator+(const Vector2& _other)const;
	// 減算
	Vector2 operator-(const Vector2& _other)const;
	// 乗算
	Vector2 operator*(float _value)const;
	/// <summary>
	/// 徐算(整数の場合static_castで整数になります)
	/// </summary>
	/// <returns>0徐算の場合は、そのままの値を返します</returns>
	Vector2 operator/(float _value)const;

	// 加算
	Vector2& operator+=(const Vector2& _other);
	// 減算
	Vector2& operator-=(const Vector2& _other);
	// 乗算
	Vector2& operator*=(float _value);
	/// <summary>
	/// 徐算(整数の場合static_castで整数になります)
	/// </summary>
	/// <returns>0徐算の場合は、そのままの値を返します</returns>
	Vector2& operator/=(float _value);

	// 逆ベクトル
	Vector2 operator-()const;

	// 等価
	bool operator==(const Vector2& _other) const;
	// 不等価
	bool operator!=(const Vector2& _other) const;

	// ベクトルの長さ(実数のみ)
	float Length() const;
	// ベクトルの長さの2乗(実数のみ)
	float LengthSqr() const;
	// 2つのベクトル間の距離(実数のみ)
	static float Distance(const Vector2& _from, const Vector2& _to);
	// 2つのベクトル間の距離の2乗(実数のみ)
	static float DistanceSqr(const Vector2& _from, const Vector2& _to);
	// 2つのベクトル間の角度(実数のみ)
	static float Angle(const Vector2& _from, const Vector2& _to);

	// 正規化(値の変化有)(実数のみ)
	Vector2& Normalize();
	// 正規化(値の変化有)(実数のみ)
	static Vector2& Normalize(Vector2& _vec);

	// 正規化(値の変化無)(実数のみ)
	Vector2 Normalized();
	// 正規化(値の変化無)(実数のみ)
	static Vector2 Normalized(const Vector2& _vec);

	// 内積
	static float Dot(const Vector2& _vec1, const Vector2& _vec2);
	// 外積
	static float Cross(const Vector2& _vec1, const Vector2& _vec2);
};