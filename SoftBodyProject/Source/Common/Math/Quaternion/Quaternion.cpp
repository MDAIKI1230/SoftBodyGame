#include <math.h>
#include "../Vector/SIMD/SIMDVectorMath.h"
#include "../Matrix/MatGenerateFunc.h"
#include "../Constants/MathConstants.h"
#include "Quaternion.h"

// 単位
const Quaternion Quaternion::IDENTITY{ 0.0f,0.0f,0.0f,1.0f };

// ベクトル回転
Vector3 Quaternion::Rotate(const Vector3& _vec) const
{
	// 虚部ベクトル
	Vector3 qv{ x, y, z };
	Vector3 t{ Vector3::Cross(qv, _vec) * 2.0f };
	return _vec + t * w + Vector3::Cross(qv, t);
}

// 正規化
Quaternion Quaternion::Normalized()
{
	Quaternion result{ simd };
	return result.Normalize();
}

// 正規化
Quaternion& Quaternion::Normalize()
{
	// 精度優先でちゃんと計算する
	float invLen{ 1.0f / sqrtf(Quaternion::Dot(*this, *this)) };
	simd = SIMDVectorMath::MulScalar(simd, invLen);
	return *this;
}

// 正規化
Quaternion Quaternion::Normalized(const Quaternion& _rot)
{
	Quaternion result{ _rot };
	return result.Normalize();
}

// 正規化
Quaternion& Quaternion::Normalize(Quaternion& _rot)
{
	return  _rot.Normalize();
}

// 乗法
Quaternion Quaternion::operator*(const Quaternion& _value) const
{
	// 実部
	float ansW{ w * _value.w - SIMDVectorMath::Dot3(simd,_value.simd) };

	// 虚部
	// w1 * v2 + w2 * v1 + v1 × v2
	SIMDVectorFloat vec
	{
		SIMDVectorMath::Add(
			SIMDVectorMath::Add(
				SIMDVectorMath::MulScalar(_value.simd,w),SIMDVectorMath::MulScalar(simd,_value.w))
			,SIMDVectorMath::Cross3(simd,_value.simd))
	};

	float elements[4];
	vec.Store(&elements[0]);

	// 実部と虚部の合成
	return Quaternion{ elements[0],elements[1],elements[2],ansW };
}

// 乗法
Quaternion Quaternion::operator*=(const Quaternion& _value)
{
	// 実部
	float ansW = w * _value.w - SIMDVectorMath::Dot3(simd, _value.simd);

	// 虚部
	// w1 * v2 + w2 * v1 + v1 × v2
	SIMDVectorFloat vec
	{
		SIMDVectorMath::Add(
			SIMDVectorMath::Add(
				SIMDVectorMath::MulScalar(_value.simd,w), // w1 * v2
				SIMDVectorMath::MulScalar(simd,_value.w)) // w2 * v1
			,SIMDVectorMath::Cross3(simd,_value.simd)) //  v1 × v2
	};

	vec.Store(&x);

	w = ansW;

	return *this;
}

// 逆数
Quaternion Quaternion::Inverse() const
{
	// 内積
	float dot{ SIMDVectorMath::Dot4(simd,simd) };
	// simd / 内積
	return SIMDVectorMath::DivScalar(this->Conjugate().simd, dot);
}

// 共役
Quaternion Quaternion::Conjugate()const
{
	return { -x,-y,-z,w };
}

// 軸と角度で回転
Quaternion Quaternion::AngleAxis(float _rad, const Vector3& _axis)
{
	if (_axis.LengthSqr() == 0.0f)
	{
		return Quaternion{};
	}
	Vector3 normal = Vector3::Normalized(_axis);

	float angleHalf{ _rad / 2.0f };

	float sin{ sinf(angleHalf) };
	float cos{ cosf(angleHalf) };

	return
	{
		normal.x * sin,
		normal.y * sin,
		normal.z * sin,
		cos
	};
}

// オイラー角から生成
Quaternion Quaternion::Euler(float _pitch, float _yaw, float _roll)
{
	// Pitch：X軸
	float cp = cosf(_pitch * 0.5f);
	float sp = sinf(_pitch * 0.5f);

	// Yaw：Y軸
	float cy = cosf(_yaw * 0.5f);
	float sy = sinf(_yaw * 0.5f);

	// Roll：Z軸
	float cr = cosf(_roll * 0.5f);
	float sr = sinf(_roll * 0.5f);

	// yaw * pitch * roll
	// 適用順序：Roll → Pitch → Yaw
	return
	{
		sp * cy * cr + cp * sy * sr, // x
		cp * sy * cr - sp * cy * sr, // y
		cp * cy * sr - sp * sy * cr, // z
		cp * cy * cr + sp * sy * sr  // w
	};
}

// 行列に変換
Matrix4x4 Quaternion::ToMatrix() const
{
	return MatGenerateFunc::Rotate(*this);
}

// 軸と角度に分解する関数
void Quaternion::ToAxisAngle(Vector3& _axis, float& _theta) const
{
	// 正規化されてる前提とする
	Quaternion q{ *this };


	if (w < 0.0f)
	{
		q = { -q.x,-q.y,-q.z,-q.w };
	}

	_axis = { q.x, q.y, q.z };

	float len{ _axis.Length() };

	// 回転がほぼねぇため適当な軸を返します。
	if (len < MathConstants::EPSILON)
	{
		_axis = Vector3::UP;
		_theta = 0.0f;
		return;
	}

	_axis /= len;
	_theta = 2.0f * atan2f(len, q.w);
}

// 球面補間
Quaternion Quaternion::Slerp(const Quaternion& _start, const Quaternion _end, float _t)
{
	float dot{ SIMDVectorMath::Dot4(_start.simd,_end.simd) };
	float theta{ acosf(dot) };

	// (sin((1-_t)θ) / sin(θ)_start + (sin(tθ) / sin(θ))_end

	float sinTheta{ sinf(theta) };
	// sin((1-_t)θ) / sin(θ)
	float weightStart{ sinf((1 - _t) * theta) / sinTheta };
	// sin(tθ) / sin(θ)
	float weightEnd{ sinf(_t * theta) / sinTheta };

	return
	{
		SIMDVectorMath::Add(
			SIMDVectorMath::MulScalar(_start.simd,weightStart), // (sin((1-_t)θ) / sin(θ)_start
			SIMDVectorMath::MulScalar(_end.simd,weightEnd)      // (sin(tθ) / sin(θ))_end
			)
	};
}


// 線形補間
Quaternion Quaternion::Lerp(const Quaternion& _start, const Quaternion _end, float _t)
{
	// start * ( 1 - t) + b * t
	return { SIMDVectorMath::Add(
		SIMDVectorMath::MulScalar(_start.simd,(1 - _t)),// start * ( 1 - t)
		SIMDVectorMath::MulScalar(_end.simd,_t)         // b * t
	) };
}

// 内積
float Quaternion::Dot(const Quaternion& _rot1, const Quaternion& _rot2)
{
	return SIMDVectorMath::Dot4(_rot1.simd, _rot2.simd);
}

Quaternion Quaternion::LookAt(const Vector3& _eye, const Vector3& _target, const Vector3& _up)
{
	const Vector3 forward{Vector3::Normalized(_target - _eye)};

	const Vector3 right{Vector3::Normalized(Vector3::Cross(Vector3::UP, forward))};

	const Vector3 up{Vector3::Cross(forward, right)};

	const Matrix4x4 rotationMatrix
	{
		right.x, up.x, forward.x, 0.0f,
		right.y, up.y, forward.y, 0.0f,
		right.z, up.z, forward.z, 0.0f,
		0.0f,    0.0f, 0.0f,      1.0f
	};

	return Quaternion::FromMatrix(rotationMatrix);
}

// 行列から四元数を作る
Quaternion Quaternion::FromMatrix(Matrix4x4 _mat)
{
	Quaternion q;
	// 対角の成分を合成
	float trace = _mat.m[0][0] + _mat.m[1][1] + _mat.m[2][2];

	if (trace > 0.0f)
	{
		float s = sqrtf(trace + 1.0f) * 2.0f; // s = 4 * w
		q.w = 0.25f * s;
		// (2yz - 2xw) - (2yz + 2xw) = 4wx
		q.x = (_mat.m[2][1] - _mat.m[1][2]) / s;
		// (2xz - 2yw) - (2xz + 2yw) = 4wy
		q.y = (_mat.m[0][2] - _mat.m[2][0]) / s;
		// (2xy - 2zx) - (2xy + 2zw) = 4wz
		q.z = (_mat.m[1][0] - _mat.m[0][1]) / s;
	}
	else if (_mat.m[0][0] > _mat.m[1][1] && _mat.m[0][0] > _mat.m[2][2])
	{
		float s = sqrtf(1.0f + _mat.m[0][0] - _mat.m[1][1] - _mat.m[2][2]) * 2.0f; // s = 4 * x
		// (2yz - 2xw) - (2yz + 2xw) = 4wx
		q.w = (_mat.m[2][1] - _mat.m[1][2]) / s;
		q.x = 0.25f * s;
		// (2xy + 2zw) + (2xy - 2 zw) = 4xy
		q.y = (_mat.m[0][1] + _mat.m[1][0]) / s;
		// (2xz + 2wy) + (2xz - 2wy) = 4xz
		q.z = (_mat.m[0][2] + _mat.m[2][0]) / s;
	}
	else if (_mat.m[1][1] > _mat.m[2][2])
	{
		float s = sqrtf(1.0f + _mat.m[1][1] - _mat.m[0][0] - _mat.m[2][2]) * 2.0f; // s = 4 * y
		// (2xz - 2yw) - (2xz + 2yw) = 4wy
		q.w = (_mat.m[0][2] - _mat.m[2][0]) / s;
		// (2xy + 2wz) + (2xy - 2wz) = 4yx
		q.x = (_mat.m[0][1] + _mat.m[1][0]) / s;
		q.y = 0.25f * s;
		// (2yz + 2wx) + (2yz - 2wx) = 4zy
		q.z = (_mat.m[1][2] + _mat.m[2][1]) / s;
	}
	else
	{
		float s = sqrtf(1.0f + _mat.m[2][2] - _mat.m[0][0] - _mat.m[1][1]) * 2.0f; // s = 4 * z
		// (2xy - 2zx) - (2xy + 2zw) = 4zw
		q.w = (_mat.m[1][0] - _mat.m[0][1]) / s;
		// (2xz - 2yw) + (2xz + 2yw) = 4zx
		q.x = (_mat.m[0][2] + _mat.m[2][0]) / s;
		// (2yz + 2xw) + (2yz - 2xw) = 4zy
		q.y = (_mat.m[1][2] + _mat.m[2][1]) / s;
		q.z = 0.25f * s;
	}

	return Normalize(q);
}
