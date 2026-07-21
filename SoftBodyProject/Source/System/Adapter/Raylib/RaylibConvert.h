#pragma once

#include "MDMath.h"
#include "Color.h"

#include "RaylibInclude.h"

// 行列変換
RlMatrix ToRaylib(const Matrix4x4& _mat)
{
	RlMatrix r;

	r.m0 = _mat.m[0][0];
	r.m1 = _mat.m[1][0];
	r.m2 = _mat.m[2][0];
	r.m3 = _mat.m[3][0];

	r.m4 = _mat.m[0][1];
	r.m5 = _mat.m[1][1];
	r.m6 = _mat.m[2][1];
	r.m7 = _mat.m[3][1];

	r.m8 = _mat.m[0][2];
	r.m9 = _mat.m[1][2];
	r.m10 = _mat.m[2][2];
	r.m11 = _mat.m[3][2];

	r.m12 = _mat.m[0][3];
	r.m13 = _mat.m[1][3];
	r.m14 = _mat.m[2][3];
	r.m15 = _mat.m[3][3];

	return r;
}

// ベクトル変換
RlVector3 ToRaylib(const Vector3& _vec)
{
	return RlVector3{ _vec.x, _vec.y, _vec.z };
}

// 色
RlColor ToRaylib(const Color& _color)
{
	return RlColor{ (unsigned char)_color.r,(unsigned char)_color.g, (unsigned char)_color.b,(unsigned char)_color.a };
}
