#pragma once

#include "MDMath.h"
#include "Color.h"
#include "GamePadConstants.h"

#include "Dxlib.h"

namespace MathConvert
{
	// 行列変換
	static DxLib::MATRIX ToDxlib(const Matrix4x4& _mat)
	{
		DxLib::MATRIX r{};

		// 転置
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
	static DxLib::VECTOR ToDxlib(const Vector3& _vec)
	{
		return DxLib::VGet(_vec.x, _vec.y, _vec.z);
	}

	// 色
	static unsigned int ToDxlib(const Color& _color)
	{
		return DxLib::GetColor(static_cast<int>(_color.r * 255), static_cast<int>(_color.g * 255), static_cast<int>(_color.b * 255));
	}

	static Matrix4x4 ToMDMath(const DxLib::MATRIX _mat)
	{
		// 転置
		DxLib::MATRIX transpose{ DxLib::MTranspose(_mat) };

		Matrix4x4 r{};

		r.m[0][0] = transpose.m[0][0];
		r.m[0][1] = transpose.m[0][1];
		r.m[0][2] = transpose.m[0][2];
		r.m[0][3] = transpose.m[0][3];

		r.m[1][0] = transpose.m[1][0];
		r.m[1][1] = transpose.m[1][1];
		r.m[1][2] = transpose.m[1][2];
		r.m[1][3] = transpose.m[1][3];

		r.m[2][0] = transpose.m[2][0];
		r.m[2][1] = transpose.m[2][1];
		r.m[2][2] = transpose.m[2][2];
		r.m[2][3] = transpose.m[2][3];

		r.m[3][0] = transpose.m[3][0];
		r.m[3][1] = transpose.m[3][1];
		r.m[3][2] = transpose.m[3][2];
		r.m[3][3] = transpose.m[3][3];

		return r;
	}

}

namespace SwitchGamePad
{
	constexpr int ConvertSwitchButton(GamePadButton button)
	{
		switch (button)
		{
		case GamePadButton::BUTTON_SOUTH:      return 0;  // B
		case GamePadButton::BUTTON_EAST:       return 1;  // A
		case GamePadButton::BUTTON_WEST:       return 2;  // Y
		case GamePadButton::BUTTON_NORTH:      return 3;  // X

		case GamePadButton::LEFT_SHOULDER:     return 4;  // L
		case GamePadButton::RIGHT_SHOULDER:    return 5;  // R

		case GamePadButton::SELECT:            return 8;  // -
		case GamePadButton::START:             return 9;  // +

		case GamePadButton::LEFT_STICK_PRESS:  return 10;
		case GamePadButton::RIGHT_STICK_PRESS: return 11;

		case GamePadButton::GUIDE:             return 12; // HOME

		default:
			return -1;
		}
	}

	constexpr int ConvertSwitchAxis1D(GamePadAxis1D axis)
	{
		switch (axis)
		{
		case GamePadAxis1D::LEFT_TRIGGER:  return 6; // ZL
		case GamePadAxis1D::RIGHT_TRIGGER: return 7; // ZR

		default:
			return -1;
		}
	}
}