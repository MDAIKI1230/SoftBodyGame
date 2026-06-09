#include "Matrix.h"
#include "../Vector/SIMD/SIMDVectorMath.h"

// 単位行列
Matrix4x4 Matrix4x4::Identity()
{
	return Matrix4x4{
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};
}

// 加算
Matrix4x4 Matrix4x4::operator+(const Matrix4x4& _other)const
{
	Matrix4x4 m;

	for (int i{ 0 }; i < 4; i++)
	{
		m.row[i] = SIMDVectorMath::Add(row[i], _other.row[i]);
	}

	return m;
}

// 減算
Matrix4x4 Matrix4x4::operator-(const Matrix4x4& _other)const
{
	Matrix4x4 m;

	for (int i{ 0 }; i < 4; i++)
	{
		m.row[i] = SIMDVectorMath::Sub(row[i], _other.row[i]);
	}

	return m;
}

// 乗算
Matrix4x4 Matrix4x4::operator*(const Matrix4x4& _other)const
{
	Matrix4x4 m;

	Matrix4x4 tm{ _other.Transposed() };
	for (int i{ 0 }; i < 4; i++)
	{
		for (int j{ 0 }; j < 4; j++)
		{
			m.m[i][j] = SIMDVectorMath::HorizontalAdd(SIMDVectorMath::Mul(row[i], tm.row[j]));
		}
	}

	return m;
}

// 加算
Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& _other)
{
	for (int i{ 0 }; i < 4; i++)
	{
		row[i] = SIMDVectorMath::Add(row[i], _other.row[i]);
	}

	return *this;
}

// 減算
Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& _other)
{
	for (int i{ 0 }; i < 4; i++)
	{
		row[i] = SIMDVectorMath::Sub(row[i], _other.row[i]);
	}

	return *this;
}

// 乗算
Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& _other)
{
	Matrix4x4 tm{ _other.Transposed() };
	for (int i{ 0 }; i < 4; i++)
	{
		for (int j{ 0 }; j < 4; j++)
		{
			m[i][j] = SIMDVectorMath::HorizontalAdd(SIMDVectorMath::Mul(row[i], tm.row[j]));
		}
	}

	return *this;
}

// スカラー倍
Matrix4x4 Matrix4x4::operator*(const float& _value) const
{
	Matrix4x4 m;

	for (int i{ 0 }; i < 4; i++)
	{
		m.row[i] = SIMDVectorMath::MulScalar(row[i], _value);
	}

	return m;
}

// スカラー割
Matrix4x4 Matrix4x4::operator/(const float& _value) const
{
	Matrix4x4 m;

	for (int i{ 0 }; i < 4; i++)
	{
		m.row[i] = SIMDVectorMath::DivScalar(row[i], _value);
	}

	return m;
}

// スカラー倍
Matrix4x4& Matrix4x4::operator*=(const float& _value)
{
	for (int i{ 0 }; i < 4; i++)
	{
		row[i] = SIMDVectorMath::MulScalar(row[i], _value);
	}

	return *this;
}

// スカラー割
Matrix4x4& Matrix4x4::operator/=(const float& _value) 
{
	for (int i{ 0 }; i < 4; i++)
	{
		row[i] = SIMDVectorMath::DivScalar(row[i], _value);
	}

	return *this;
}

// 転置(変化しない)
Matrix4x4 Matrix4x4::Transposed() const
{
	SIMDVectorFloat temp1{ SIMDVectorFloat::UnpackLow(row[0],row[1]) };  // m00,m10,m01,m11
	SIMDVectorFloat temp2{ SIMDVectorFloat::UnpackHigh(row[0],row[1]) }; // m02,m12,m03,m13
	SIMDVectorFloat temp3{ SIMDVectorFloat::UnpackLow(row[2],row[3]) };  // m20,m30,m21,m31
	SIMDVectorFloat temp4{ SIMDVectorFloat::UnpackHigh(row[2],row[3]) }; // m22,m32,m23,m33

	return Matrix4x4
	{
		SIMDVectorFloat::MoveLow(temp1,temp3),
		SIMDVectorFloat::MoveHigh(temp1,temp3),
		SIMDVectorFloat::MoveLow(temp2,temp4),
		SIMDVectorFloat::MoveHigh(temp2,temp4)
	};
}

// 転置
Matrix4x4& Matrix4x4::Transpose()
{
	SIMDVectorFloat temp1{ SIMDVectorFloat::UnpackLow(row[0],row[1]) };  // m00,m10,m01,m11
	SIMDVectorFloat temp2{ SIMDVectorFloat::UnpackHigh(row[0],row[1]) }; // m02,m12,m03,m13
	SIMDVectorFloat temp3{ SIMDVectorFloat::UnpackLow(row[2],row[3]) };  // m20,m30,m21,m31
	SIMDVectorFloat temp4{ SIMDVectorFloat::UnpackHigh(row[2],row[3]) }; // m22,m32,m23,m33

	row[0] = SIMDVectorFloat::MoveLow(temp1, temp3);
	row[1] = SIMDVectorFloat::MoveHigh(temp1, temp3);
	row[2] = SIMDVectorFloat::MoveLow(temp2, temp4);
	row[3] = SIMDVectorFloat::MoveHigh(temp2, temp4);

	return *this;
}

// 転置(変化しない)
Matrix4x4 Matrix4x4::Transposed(Matrix4x4& _value)
{
	SIMDVectorFloat temp1{ SIMDVectorFloat::UnpackLow(_value.row[0],_value.row[1]) };  // m00,m10,m01,m11
	SIMDVectorFloat temp2{ SIMDVectorFloat::UnpackHigh(_value.row[0],_value.row[1]) }; // m02,m12,m03,m13
	SIMDVectorFloat temp3{ SIMDVectorFloat::UnpackLow(_value.row[2],_value.row[3]) };  // m20,m30,m21,m31
	SIMDVectorFloat temp4{ SIMDVectorFloat::UnpackHigh(_value.row[2],_value.row[3]) }; // m22,m32,m23,m33

	return Matrix4x4
	{
		SIMDVectorFloat::MoveLow(temp1,temp3),
		SIMDVectorFloat::MoveHigh(temp1,temp3),
		SIMDVectorFloat::MoveLow(temp2,temp4),
		SIMDVectorFloat::MoveHigh(temp2,temp4)
	};
}

// 転置
Matrix4x4& Matrix4x4::Transpose(Matrix4x4& _value)
{
	SIMDVectorFloat temp1{ SIMDVectorFloat::UnpackLow(_value.row[0],_value.row[1]) };  // m00,m10,m01,m11
	SIMDVectorFloat temp2{ SIMDVectorFloat::UnpackHigh(_value.row[0],_value.row[1]) }; // m02,m12,m03,m13
	SIMDVectorFloat temp3{ SIMDVectorFloat::UnpackLow(_value.row[2],_value.row[3]) };  // m20,m30,m21,m31
	SIMDVectorFloat temp4{ SIMDVectorFloat::UnpackHigh(_value.row[2],_value.row[3]) }; // m22,m32,m23,m33

	_value.row[0] = SIMDVectorFloat::MoveLow(temp1, temp3);
	_value.row[1] = SIMDVectorFloat::MoveHigh(temp1, temp3);
	_value.row[2] = SIMDVectorFloat::MoveLow(temp2, temp4);
	_value.row[3] = SIMDVectorFloat::MoveHigh(temp2, temp4);

	return _value;
}

// 一次変換
Vector3 Matrix4x4::operator *(const Vector3& _other) const 
{
	SIMDVectorFloat vec{ _other.x,_other.y ,_other.z ,1.0f };
	
	return Vector3{
		SIMDVectorMath::Dot4(row[0],vec),
		SIMDVectorMath::Dot4(row[1],vec),
		SIMDVectorMath::Dot4(row[2],vec)
	};
}