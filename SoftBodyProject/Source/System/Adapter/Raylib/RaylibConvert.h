#pragma once

#include "MDMath.h"
#include "Color.h"

#include "RaylibInclude.h"

// 行列変換
RlMatrix ToRaylib(const Matrix4x4& _mat)
{
	const Matrix4x4 flipZ{ MatGenerateFunc::Scale(Vector3{ 1.0f, 1.0f, -1.0f }) };
	
	const Matrix4x4 converted{ flipZ * _mat * flipZ };

	RlMatrix r;

	r.m0 = converted.m[0][0];
	r.m1 = converted.m[1][0];
	r.m2 = converted.m[2][0];
	r.m3 = converted.m[3][0];

	r.m4 = converted.m[0][1];
	r.m5 = converted.m[1][1];
	r.m6 = converted.m[2][1];
	r.m7 = converted.m[3][1];

	r.m8 = converted.m[0][2];
	r.m9 = converted.m[1][2];
	r.m10 = converted.m[2][2];
	r.m11 = converted.m[3][2];

	r.m12 = converted.m[0][3];
	r.m13 = converted.m[1][3];
	r.m14 = converted.m[2][3];
	r.m15 = converted.m[3][3];

	return r;
}

// ベクトル変換
RlVector3 ToRaylib(const Vector3& _vec)
{
	return RlVector3{ _vec.x, _vec.y, -_vec.z };
}

// 色
RlColor ToRaylib(const Color& _color)
{
	return RlColor{ 
		static_cast<unsigned char>(_color.r * 255),
		static_cast<unsigned char>(_color.g * 255),
		static_cast<unsigned char>(_color.b * 255),
		static_cast<unsigned char>(_color.a * 255) };
}
