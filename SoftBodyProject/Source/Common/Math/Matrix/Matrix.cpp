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

// 逆行列(変化しない)
Matrix4x4 Matrix4x4::Inversed() const
{
	Matrix4x4 result{ *this };
	return Inverse(result);
}

// 逆行列
Matrix4x4& Matrix4x4::Inverse()
{
	return Inverse(*this);
}

// 逆行列(変化しない)
Matrix4x4 Matrix4x4::Inversed(Matrix4x4& _value)
{
	return _value;
}

// 逆行列
Matrix4x4& Matrix4x4::Inverse(Matrix4x4& _value)
{
	/*
		元の行列 =　｜a, b, c, d｜
					｜e, f, g, h｜
					｜i, j, k, l｜
					｜m, n, o, p｜

	*/

	/*
		転置行列 =  |a, e, i, m|
					|b, f, j, n|
					|c, g, k, o|
					|d, h, l, p|
	*/  
	// --- 2×2行列式の計算を一気にやる ---
	// 列を(0, 1)と(2, 3)に固定して重複を除くすべてのパターンの計算をする
	// 行は(0, 1)(0, 2)(0, 3)(1, 2)(1, 3)(2, 3)がすべてのパターン

	// {a, a, b, b}
	SIMDVectorFloat vec00{ SIMDVectorFloat::Shuffle<0, 0, 1, 1>(_value.row[0]) };
	// {g, h, g, h}
	SIMDVectorFloat vec01{ SIMDVectorFloat::Shuffle<2, 3, 2, 3>(_value.row[1]) };

	// {i, i, j, j}
	SIMDVectorFloat vec10{ SIMDVectorFloat::Shuffle<0, 0, 1, 1>(_value.row[2]) };
	// {o, p, o, p}
	SIMDVectorFloat vec11{ SIMDVectorFloat::Shuffle<2, 3, 2, 3>(_value.row[3]) };
	
	// {a, c, i, k}
	SIMDVectorFloat vec20{ SIMDVectorFloat::Shuffle<0, 2, 0, 2>(_value.row[0],_value.row[2]) };
	// {f, h, n, p}
	SIMDVectorFloat vec21{ SIMDVectorFloat::Shuffle<1, 3, 1, 3>(_value.row[1],_value.row[3]) };
	
	// {ag, ah, bg, bh}
	SIMDVectorFloat det0{ SIMDVectorMath::Mul(vec00,vec01) };
	// {io, ip, jo, jp}
	SIMDVectorFloat det1{ SIMDVectorMath::Mul(vec10,vec11) };
	// {af, ch, in, kp}
	SIMDVectorFloat det2{ SIMDVectorMath::Mul(vec20,vec21) };

	// {e, e, f, f}
	vec00 = SIMDVectorFloat::Shuffle<0, 0, 1, 1>(_value.row[1]);
	// {c, d, c, d}
	vec01 = SIMDVectorFloat::Shuffle<2, 3, 2, 3>(_value.row[0]);

	// {m, m, n, n}
	vec10 = SIMDVectorFloat::Shuffle<0, 0, 1, 1>(_value.row[3]);
	// {k, l, k, l}
	vec11 = SIMDVectorFloat::Shuffle<2, 3, 2, 3>(_value.row[2]);

	// {e, g, m, o}
	vec20 = SIMDVectorFloat::Shuffle<0, 2, 0, 2>(_value.row[1], _value.row[3]);
	// {b, d, j, l}
	vec21 = SIMDVectorFloat::Shuffle<1, 3, 1, 3>(_value.row[0], _value.row[2]);

	// {ag - ec, ah - ed, bg - fc, bh - fd}列(0, 1)固定行(0, 2)(0, 3)(1, 2)(1, 3)
	det0 = SIMDVectorMath::Sub(
		// {ec, ed, fc, fd}
		det0, SIMDVectorMath::Mul(vec00, vec01));
	// {io - mk, ip - ml, jo - nk, jp - nl}列(2, 3)固定(0, 2)(0, 3)(1, 2)(1, 3)
	det1 = SIMDVectorMath::Sub(
		// {mk, ml, nk, nl}
		det1, SIMDVectorMath::Mul(vec10, vec11));
	// {af - eb, ch - gd, in - mj, kp - ol}対角に接する奴ら
	det2 = SIMDVectorMath::Sub(
		// {eb, gd, mj, ol}
		det2, SIMDVectorMath::Mul(vec20, vec21));

	// 
	
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
