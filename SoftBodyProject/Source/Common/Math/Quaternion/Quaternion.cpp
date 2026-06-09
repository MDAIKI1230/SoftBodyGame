#include <math.h>
#include "../Vector/SIMD/SIMDVectorMath.h"
#include "../Matrix/MatGenerateFunc.h"
#include "Quaternion.h"

// 単位
Quaternion Quaternion::Identity()
{
	return Quaternion{ 0,0,0,1 };
}

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
	return SIMDVectorMath::Normalize(simd);
}

// 正規化
Quaternion& Quaternion::Normalize()
{
	simd = SIMDVectorMath::Normalize(simd);
	return *this;
}

// 正規化
Quaternion Quaternion::Normalized(Quaternion& _rot)
{
	return SIMDVectorMath::Normalize(_rot.simd);
}

// 正規化
Quaternion& Quaternion::Normalize(Quaternion& _rot)
{
	_rot.simd = SIMDVectorMath::Normalize(_rot.simd);
	return _rot;
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
	// ノルム
	float norm{ sqrtf(dot) };
	// simd / norm
	return SIMDVectorMath::DivScalar(this->Conjugate().simd, norm);
}

// 共役
Quaternion Quaternion::Conjugate()const
{
	return { -x,-y,-z,w };
}

// 軸と角とで回転
Quaternion Quaternion::AngleAxis(float _rad, const Vector3& _axis)
{
	float angleHalf{ _rad / 2.0f };

	float sin{ sinf(angleHalf) };
	float cos{ cosf(angleHalf) };

	return
	{
		_axis.x * sin,
		_axis.y * sin,
		_axis.z * sin,
		cos
	};
}

// オイラー角から生成
Quaternion Quaternion::Euler(float _pitch, float _yaw, float _roll)
{
	// yaw
	float cy = cosf(_yaw * 0.5f);
	float sy = sinf(_yaw * 0.5f);
	// pitch
	float cp = cosf(_pitch * 0.5f);
	float sp = sinf(_pitch * 0.5f);
	// roll
	float cr = cosf(_roll * 0.5f);
	float sr = sinf(_roll * 0.5f);

	return
	{
		cr * cp * cy + sr * sp * sy,
		sr * cp * cy - cr * sp * sy,
		cr * sp * cy + sr * cp * sy,
		cr * cp * sy - sr * sp * cy
	};
}

// 行列に変換
Matrix4x4 Quaternion::ToMatrix() const
{
	return MatGenerateFunc::Rotate(*this);
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
	return FromMatrix(MatGenerateFunc::LookAt(_eye, _target, _up));
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
		// (2yz + 2xw) - (2yz - 2xw) = 4wx
		q.x = (_mat.m[1][2] - _mat.m[2][1]) / s;
		// (2xz + 2yw) - (2xz - 2yw) = 4wy
		q.y = (_mat.m[2][0] - _mat.m[0][2]) / s;
		// (2xy + 2zw) - (2xy - 2zx) = 4wz
		q.z = (_mat.m[0][1] - _mat.m[1][0]) / s;
	}
	else if (_mat.m[0][0] > _mat.m[1][1] && _mat.m[0][0] > _mat.m[2][2])
	{
		float s = sqrtf(1.0f + _mat.m[0][0] - _mat.m[1][1] - _mat.m[2][2]) * 2.0f; // s = 4 * x
		// (2yz + 2xw) - (2yz - 2xw) = 4wx
		q.w = (_mat.m[1][2] - _mat.m[2][1]) / s;
		q.x = 0.25f * s;
		// (2xy + 2zw) + (2xy - 2 zw) = 4xy
		q.y = (_mat.m[0][1] + _mat.m[1][0]) / s;
		// (2xz + 2wy) + (2xz - 2wy) = 4xz
		q.z = (_mat.m[0][2] + _mat.m[2][0]) / s;
	}
	else if (_mat.m[1][1] > _mat.m[2][2])
	{
		float s = sqrtf(1.0f + _mat.m[1][1] - _mat.m[0][0] - _mat.m[2][2]) * 2.0f; // s = 4 * y
		// (2xz + 2yw) - (2xz - 2yw) = 4wy
		q.w = (_mat.m[2][0] - _mat.m[0][2]) / s;
		// (2xy + 2wz) + (2xy - 2wz) = 4yx
		q.x = (_mat.m[0][1] + _mat.m[1][0]) / s;
		q.y = 0.25f * s;
		// (2yz + 2wx) + (2yz - 2wx) = 4zy
		q.z = (_mat.m[1][2] + _mat.m[2][1]) / s;
	}
	else
	{
		float s = sqrtf(1.0f + _mat.m[2][2] - _mat.m[0][0] - _mat.m[1][1]) * 2.0f; // s = 4 * z
		// (2xy + 2zw) - (2xy - 2zx) = 4zw
		q.w = (_mat.m[0][1] - _mat.m[1][0]) / s;
		// (2xz - 2yw) + (2xz + 2yw) = 4zx
		q.x = (_mat.m[0][2] + _mat.m[2][0]) / s;
		// (2yz + 2xw) + (2yz - 2xw) = 4zy
		q.y = (_mat.m[1][2] + _mat.m[2][1]) / s;
		q.z = 0.25f * s;
	}

	return Normalize(q);
}