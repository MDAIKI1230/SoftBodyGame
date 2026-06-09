#include <math.h>
#include "../SIMD/SIMDVectorMath.h"
#include "Vector2.h"

const Vector2 Vector2::ZERO{ 0.0f,0.0f };
const Vector2 Vector2::ONE{ 1.0f,1.0f };
const Vector2 Vector2::UP{ 0.0f,1.0f };
const Vector2 Vector2::RIGHT{ 1.0f,0.0f };

// 加算
Vector2 Vector2::operator+(const Vector2& _other) const
{
	return SIMDVectorMath::Add(simd, _other.simd);
}

// 減算
Vector2 Vector2::operator-(const Vector2& _other)const
{
	return SIMDVectorMath::Sub(simd, _other.simd);
}

// 乗算
Vector2 Vector2::operator*(float _value)const
{
	return SIMDVectorMath::MulScalar(simd, _value);
}

/// <summary>
/// 徐算(整数の場合static_castで整数になります)
/// </summary>
/// <returns>0徐算の場合は、そのままの値を返します</returns>
Vector2 Vector2::operator/(float _value)const
{
	return SIMDVectorMath::DivScalar(simd, _value);
}

// 加算
Vector2& Vector2::operator+=(const Vector2& _other)
{
	simd = SIMDVectorMath::Add(simd, _other.simd);
	return *this;
}

// 減算
Vector2& Vector2::operator-=(const Vector2& _other)
{
	simd = SIMDVectorMath::Sub(simd, _other.simd);
	return *this;
}

// 乗算
Vector2& Vector2::operator*=(float _value)
{
	simd = SIMDVectorMath::MulScalar(simd, _value);
	return *this;
}

/// <summary>
/// 徐算(整数の場合static_castで整数になります)
/// </summary>
/// <returns>0徐算の場合は、そのままの値を返します</returns>
Vector2& Vector2::operator/=(float _value)
{
	simd = SIMDVectorMath::DivScalar(simd, _value);
	return *this;
}

// 逆ベクトル
Vector2 Vector2::operator-()const
{
	return SIMDVectorMath::Negate(simd);
}

// 等価
bool Vector2::operator==(const Vector2& _other) const
{
	return SIMDVectorMath::Equals(simd, _other.simd);
}

// 不等価
bool Vector2::operator!=(const Vector2& _other) const
{
	return !SIMDVectorMath::Equals(simd, _other.simd);
}

// ベクトルの長さ(実数のみ)
float Vector2::Length() const
{
	return SIMDVectorMath::Length2(simd);
}

// ベクトルの長さの2乗(実数のみ)
float Vector2::LengthSqr() const
{
	return SIMDVectorMath::Dot2(simd, simd);
}

// 2つのベクトル間の距離(実数のみ)
float Vector2::Distance(const Vector2& _from, const Vector2& _to)
{
	return SIMDVectorMath::Length2((_to - _from).simd);
}

// 2つのベクトル間の距離の2乗(実数のみ)
float Vector2::DistanceSqr(const Vector2& _from, const Vector2& _to)
{
	Vector2 dist{ _to - _from };
	return SIMDVectorMath::Dot2(dist, dist);
}

// 2つのベクトル間の角度(実数のみ)
float Vector2::Angle(const Vector2& _from, const Vector2& _to)
{
	float fromLen = _from.Length();
	float toLen = _to.Length();

	if (fromLen == 0 || toLen == 0)
	{
		return 0.0f;
	}

	return acosf(Dot(_from, _to) / (fromLen * toLen));
}

// 正規化(値の変化有)(実数のみ)
Vector2& Vector2::Normalize()
{
	simd = SIMDVectorMath::Normalize(simd);
	return *this;
}

// 正規化(値の変化有)(実数のみ)
Vector2& Vector2::Normalize(Vector2& _vec)
{
	_vec.simd = SIMDVectorMath::Normalize(_vec);
	return _vec;
}

// 正規化(値の変化無)(実数のみ)
Vector2 Vector2::Normalized()
{
	return SIMDVectorMath::Normalize(simd);
}

// 正規化(値の変化無)(実数のみ)
Vector2 Vector2::Normalized(const Vector2& _vec)
{
	return SIMDVectorMath::Normalize(_vec.simd);
}

// 内積
float Vector2::Dot(const Vector2& _vec1, const Vector2& _vec2)
{
	return SIMDVectorMath::Dot2(_vec1, _vec2);
}

// 外積
float Vector2::Cross(const Vector2& _vec1, const Vector2& _vec2)
{
	return _vec1.x * _vec2.y - _vec2.x * _vec1.y;
}