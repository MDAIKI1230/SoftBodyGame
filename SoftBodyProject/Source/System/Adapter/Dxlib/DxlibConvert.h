#pragma once

#include "MDMath.h"
#include "Color.h"

#include "Dxlib.h"

// 行列変換
DxLib::MATRIX ToDxlib(const Matrix4x4& _mat)
{
	DxLib::MATRIX r{};

	// 行優先に変換
	r.m[0][0] = _mat.m[0][0];
	r.m[0][1] = _mat.m[1][0];
	r.m[0][2] = _mat.m[2][0];
	r.m[0][3] = _mat.m[3][0];

	r.m[1][0] = _mat.m[0][1];
	r.m[1][1] = _mat.m[1][1];
	r.m[1][2] = _mat.m[2][1];
	r.m[1][3] = _mat.m[3][1];

	r.m[2][0] = _mat.m[0][2];
	r.m[2][1] = _mat.m[1][2];
	r.m[2][2] = _mat.m[2][2];
	r.m[2][3] = _mat.m[3][2];

	r.m[3][0] = _mat.m[0][3];
	r.m[3][1] = _mat.m[1][3];
	r.m[3][2] = _mat.m[2][3];
	r.m[3][3] = _mat.m[3][3];

	return r;
}

// ベクトル変換
DxLib::VECTOR ToDxlib(const Vector3& _vec)
{
	return DxLib::VGet(_vec.x, _vec.y, _vec.z);
}

// 色
unsigned int ToDxlib(const Color& _color)
{
	return DxLib::GetColor(static_cast<int>(_color.r * 255), static_cast<int>(_color.g * 255), static_cast<int>(_color.b * 255));
}
