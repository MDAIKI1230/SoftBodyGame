#include "../SIMD/SIMDVectorMath.h"
#include "Vector2Int.h"

const Vector2Int Vector2Int::ZERO{ 0,0 };
const Vector2Int Vector2Int::ONE{ 1,1 };
const Vector2Int Vector2Int::UP{ 0,1 };
const Vector2Int Vector2Int::RIGHT{ 1,0 };

// 加算
Vector2Int Vector2Int::operator+(const Vector2Int& _other) const
{
	return SIMDVectorMath::Add(simd, _other.simd);
}

// 減算
Vector2Int Vector2Int::operator-(const Vector2Int& _other)const
{
	return SIMDVectorMath::Sub(simd, _other.simd);
}

// 乗算
Vector2Int Vector2Int::operator*(int _value)const
{
	return SIMDVectorMath::MulScalar(simd, _value);
}

// 加算
Vector2Int& Vector2Int::operator+=(const Vector2Int& _other)
{
	simd = SIMDVectorMath::Add(simd, _other.simd);
	return *this;
}

// 減算
Vector2Int& Vector2Int::operator-=(const Vector2Int& _other)
{
	simd = SIMDVectorMath::Sub(simd, _other.simd);
	return *this;
}

// 乗算
Vector2Int& Vector2Int::operator*=(int _value)
{
	simd = SIMDVectorMath::MulScalar(simd, _value);
	return *this;
}

// 等価
bool Vector2Int::operator==(const Vector2Int& _other) const
{
	return SIMDVectorMath::Equals(simd, _other.simd);
}

// 不等価
bool Vector2Int::operator!=(const Vector2Int& _other) const
{
	return !SIMDVectorMath::Equals(simd, _other.simd);
}