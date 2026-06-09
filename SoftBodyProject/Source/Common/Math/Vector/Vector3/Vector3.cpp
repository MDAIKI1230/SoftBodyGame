#include <math.h>
#include "../SIMD/SIMDVectorMath.h"
#include "Vector3.h"

// 0
static const Vector3 ZERO{ 0.0f,0.0f,0.0f };
// 1
static const Vector3 ONE{ 1.0f,1.0f,1.0f };
// 前(z)
const Vector3 Vector3::FORWARD{ 0.0f,0.0f,1.0f };
// 右(x)
const Vector3 Vector3::RIGHT{ 1.0f,0.0f,0.0f };
// 上(y)
const Vector3 Vector3::UP{ 0.0f,1.0f,0.0f };

// 加算
Vector3 Vector3::operator+(const Vector3& _other) const
{
	return SIMDVectorMath::Add(simd, _other.simd);
}

// 減算
Vector3 Vector3::operator-(const Vector3& _other)const
{
	return SIMDVectorMath::Sub(simd, _other.simd);
}

// 乗算
Vector3 Vector3::operator*(float _value)const
{
	return SIMDVectorMath::MulScalar(simd, _value);
}

/// <summary>
/// 徐算(整数の場合static_castで整数になります)
/// </summary>
/// <returns>0徐算の場合は、そのままの値を返します</returns>
Vector3 Vector3::operator/(float _value)const
{
	return SIMDVectorMath::DivScalar(simd, _value);
}

// 加算
Vector3& Vector3::operator+=(const Vector3& _other)
{
	simd = SIMDVectorMath::Add(simd, _other.simd);
	return *this;
}

// 減算
Vector3& Vector3::operator-=(const Vector3& _other)
{
	simd = SIMDVectorMath::Sub(simd, _other.simd);
	return *this;
}

// 乗算
Vector3& Vector3::operator*=(float _value)
{
	simd = SIMDVectorMath::MulScalar(simd, _value);
	return *this;
}

/// <summary>
/// 徐算(整数の場合static_castで整数になります)
/// </summary>
/// <returns>0徐算の場合は、そのままの値を返します</returns>
Vector3& Vector3::operator/=(float _value)
{
	simd = SIMDVectorMath::DivScalar(simd, _value);
	return *this;
}

// 逆ベクトル
Vector3 Vector3::operator-()const
{
	return SIMDVectorMath::Negate(simd);
}

// 等価
bool Vector3::operator==(const Vector3& _other) const
{
	return SIMDVectorMath::Equals(simd, _other.simd);
}

// 不等価
bool Vector3::operator!=(const Vector3& _other) const
{
	return !SIMDVectorMath::Equals(simd, _other.simd);
}

// ベクトルの長さ(実数のみ)
float Vector3::Length() const
{
	return SIMDVectorMath::Length2(simd);
}

// ベクトルの長さの2乗(実数のみ)
float Vector3::LengthSqr() const
{
	return SIMDVectorMath::Dot2(simd, simd);
}

// 2つのベクトル間の距離(実数のみ)
float Vector3::Distance(const Vector3& _from, const Vector3& _to)
{
	return SIMDVectorMath::Length2((_to - _from).simd);
}

// 2つのベクトル間の距離の2乗(実数のみ)
float Vector3::DistanceSqr(const Vector3& _from, const Vector3& _to)
{
	Vector3 dist{ _to - _from };
	return SIMDVectorMath::Dot2(dist, dist);
}

// 2つのベクトル間の角度(実数のみ)
float Vector3::Angle(const Vector3& _from, const Vector3& _to)
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
Vector3& Vector3::Normalize()
{
	simd = SIMDVectorMath::Normalize(simd);
	return *this;
}

// 正規化(値の変化有)(実数のみ)
Vector3& Vector3::Normalize(Vector3& _vec)
{
	_vec.simd = SIMDVectorMath::Normalize(_vec);
	return _vec;
}

// 正規化(値の変化無)(実数のみ)
Vector3 Vector3::Normalized()
{
	return SIMDVectorMath::Normalize(simd);
}

// 正規化(値の変化無)(実数のみ)
Vector3 Vector3::Normalized(const Vector3& _vec)
{
	return SIMDVectorMath::Normalize(_vec.simd);
}

// 内積
float Vector3::Dot(const Vector3& _vec1, const Vector3& _vec2)
{
	return SIMDVectorMath::Dot2(_vec1, _vec2);
}

// 外積
Vector3 Vector3::Cross(const Vector3& _vec1, const Vector3& _vec2)
{
	return SIMDVectorMath::Cross3(_vec1, _vec2);
}