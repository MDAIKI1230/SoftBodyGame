#pragma once

#include "../SIMD/SIMDVector.h"

struct alignas(16) Vector3Int
{
public:
	union
	{
		// アクセス用数値
		struct
		{
			int x;
			int y;
			int z;
			int pad;
		};

		// SIMD計算用
		SIMDVectorInt simd;
	};

public:
	// 0
	static const Vector3Int ZERO;
	// 1
	static const Vector3Int ONE;
	// 前(z)
	static const Vector3Int FORWARD;
	// 右(x)
	static const Vector3Int RIGHT;
	// 上(y)
	static const Vector3Int UP;

public:
	// コンストラクタ
	Vector3Int() :
		simd{ 0,0,0,0 }
	{
	}

	/// <param name="simd">SIMD</param>
	Vector3Int(SIMDVectorInt simd) :
		simd(simd)
	{
	}

	/// <param name="_x">X</param>
	/// <param name="_y">Y</param>
	Vector3Int(int _x, int _y,int _z) :
		simd(_x, _y, _z, 0)
	{
	}

	// 型変換
	operator SIMDVectorInt() const
	{
		return simd;
	}

	// 加算
	Vector3Int operator+(const Vector3Int& _other)const;
	// 減算
	Vector3Int operator-(const Vector3Int& _other)const;
	// 乗算
	Vector3Int operator*(int _value)const;

	// 加算
	Vector3Int& operator+=(const Vector3Int& _other);
	// 減算
	Vector3Int& operator-=(const Vector3Int& _other);
	// 乗算
	Vector3Int& operator*=(int _value);

	// 等価
	bool operator==(const Vector3Int& _other) const;
	// 不等価
	bool operator!=(const Vector3Int& _other) const;
};