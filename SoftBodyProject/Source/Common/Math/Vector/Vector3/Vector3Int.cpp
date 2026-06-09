#include "../SIMD/SIMDVectorMath.h"
#include "Vector3Int.h"

// 0
const Vector3Int Vector3Int::ZERO{ 0,0,0 };
// 1
const Vector3Int Vector3Int::ONE{ 1,1,1 };
// 前(z)
const Vector3Int Vector3Int::FORWARD{ 0,0,1 };
// 右(x)
const Vector3Int Vector3Int::RIGHT{ 1,0,0 };
// 上(y)
const Vector3Int Vector3Int::UP{ 0,1,0 };

// 加算
Vector3Int Vector3Int::operator+(const Vector3Int& _other) const
{
	return SIMDVectorMath::Add(simd, _other.simd);
}

// 減算
Vector3Int Vector3Int::operator-(const Vector3Int& _other)const
{
	return SIMDVectorMath::Sub(simd, _other.simd);
}

// 乗算
Vector3Int Vector3Int::operator*(int _value)const
{
	return SIMDVectorMath::MulScalar(simd, _value);
}

// 加算
Vector3Int& Vector3Int::operator+=(const Vector3Int& _other)
{
	simd = SIMDVectorMath::Add(simd, _other.simd);
	return *this;
}

// 減算
Vector3Int& Vector3Int::operator-=(const Vector3Int& _other)
{
	simd = SIMDVectorMath::Sub(simd, _other.simd);
	return *this;
}

// 乗算
Vector3Int& Vector3Int::operator*=(int _value)
{
	simd = SIMDVectorMath::MulScalar(simd, _value);
	return *this;
}

// 等価
bool Vector3Int::operator==(const Vector3Int& _other) const
{
	return SIMDVectorMath::Equals(simd, _other.simd);
}

// 不等価
bool Vector3Int::operator!=(const Vector3Int& _other) const
{
	return !SIMDVectorMath::Equals(simd, _other.simd);
}