#include "../SIMD/SIMDVectorMath.h"

#include "Vector4.h"

// 加算
Vector4 Vector4::operator+(const Vector4& _other) const
{
	return SIMDVectorMath::Add(simd, _other.simd);
}

// 減算
Vector4 Vector4::operator-(const Vector4& _other)const
{
	return SIMDVectorMath::Sub(simd, _other.simd);
}

// 乗算
Vector4 Vector4::operator*(float _value)const
{
	return SIMDVectorMath::MulScalar(simd, _value);
}

/// <summary>
/// 徐算(整数の場合static_castで整数になります)
/// </summary>
/// <returns>0徐算の場合は、そのままの値を返します</returns>
Vector4 Vector4::operator/(float _value)const
{
	return SIMDVectorMath::DivScalar(simd, _value);
}

// 加算
Vector4& Vector4::operator+=(const Vector4& _other)
{
	simd = SIMDVectorMath::Add(simd, _other.simd);
	return *this;
}

// 減算
Vector4& Vector4::operator-=(const Vector4& _other)
{
	simd = SIMDVectorMath::Sub(simd, _other.simd);
	return *this;
}

// 乗算
Vector4& Vector4::operator*=(float _value)
{
	simd = SIMDVectorMath::MulScalar(simd, _value);
	return *this;
}

/// <summary>
/// 徐算(整数の場合static_castで整数になります)
/// </summary>
/// <returns>0徐算の場合は、そのままの値を返します</returns>
Vector4& Vector4::operator/=(float _value)
{
	simd = SIMDVectorMath::DivScalar(simd, _value);
	return *this;
}

// 逆ベクトル
Vector4 Vector4::operator-()const
{
	return SIMDVectorMath::Negate(simd);
}

// 等価
bool Vector4::operator==(const Vector4& _other) const
{
	return SIMDVectorMath::Equals(simd, _other.simd);
}

// 不等価
bool Vector4::operator!=(const Vector4& _other) const
{
	return !SIMDVectorMath::Equals(simd, _other.simd);
}

// ノルム
float Vector4::Norm() const
{
	return SIMDVectorMath::Norm(simd);
}

// ノルム
float Vector4::Norm(const Vector4& _vec)
{
	return SIMDVectorMath::Norm(_vec.simd);
}

// 正規化(値の変化有)(実数のみ)
Vector4& Vector4::Normalize()
{
	simd = SIMDVectorMath::Normalize(simd);
	return *this;
}

// 正規化(値の変化有)(実数のみ)
Vector4& Vector4::Normalize(Vector4& _vec)
{
	_vec.simd = SIMDVectorMath::Normalize(_vec);
	return _vec;
}

// 正規化(値の変化無)(実数のみ)
Vector4 Vector4::Normalized() const
{
	return SIMDVectorMath::Normalize(simd);
}

// 正規化(値の変化無)(実数のみ)
Vector4 Vector4::Normalized(const Vector4& _vec)
{
	return SIMDVectorMath::Normalize(_vec.simd);
}

// 内積
float Vector4::Dot(const Vector4& _vec1, const Vector4& _vec2)
{
	return SIMDVectorMath::Dot4(_vec1, _vec2);
}
