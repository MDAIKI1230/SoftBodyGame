#pragma once

#include "../Matrix/Matrix.h"

struct alignas(16) Quaternion
{
public:
	union
	{
		struct
		{
			float  x, y, z, w;
		};

		SIMDVectorFloat simd;
	};
public:
	// コンストラクタ
	Quaternion() :
		simd{ 0.0f,0.0f, 0.0f, 1.0f }
	{
	}

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_x">虚部1</param>
	/// <param name="_y">虚部2</param>
	/// <param name="_z">虚部3</param>
	/// <param name="_w">実部</param>
	Quaternion(float _x, float _y, float _z, float _w) :
		simd(_x, _y, _z, _w)
	{
	}

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="_simd">SIMD</param>
	Quaternion(SIMDVectorFloat _simd) :
		simd{ _simd }
	{
	}

	// 単位
	static Quaternion Identity();

	// ベクトル回転
	Vector3 Rotate(const Vector3& _vec) const;

	// 正規化(変化なし)
	Quaternion Normalized();
	// 正規化(変化あり)
	Quaternion& Normalize();
	// 正規化(変化なし)
	static Quaternion Normalized(Quaternion& _rot);
	// 正規化(変化あり)
	static Quaternion& Normalize(Quaternion& _rot);

	// 乗法
	Quaternion operator*(const Quaternion& _value) const;

	// 乗法
	Quaternion operator*=(const Quaternion& _value);

	// 逆数
	Quaternion Inverse() const;

	// 共役
	Quaternion Conjugate()const;

	// 軸と角とで回転
	static Quaternion AngleAxis(float _rad, const Vector3& _axis);
	// オイラー角から生成
	static Quaternion Euler(float _pitch, float _yaw, float _roll);
	// 行列に変換
	Matrix4x4 ToMatrix() const;

	/// <summary>
	/// 球面補間
	/// </summary>
	/// <param name="_rot1">スタート</param>
	/// <param name="_rot2">ゴール</param>
	/// <param name="_t">時間(0～1)</param>
	/// <returns></returns>
	static Quaternion Slerp(const Quaternion& _start, const Quaternion _end, float _t);

	/// <summary>
	/// 線形補間
	/// </summary>
	/// <param name="_rot1">スタート</param>
	/// <param name="_rot2">ゴール</param>
	/// <param name="_t">時間(0～1)</param>
	/// <returns></returns>
	static Quaternion Lerp(const Quaternion& _start, const Quaternion _end, float _t);

	// 内積
	static float Dot(const Quaternion& _rot1,const Quaternion& _rot2);

	/// <summary>
	/// どこかを見る回転を作る
	/// </summary>
	/// <param name="_eye">向けるオブジェクトの位置</param>
	/// <param name="_target">見る位置</param>
	/// <param name="_up">上</param>
	/// <returns></returns>
	static Quaternion LookAt(const Vector3& _eye, const Vector3& _target, const Vector3& _up);

	// 行列から四元数を作る
	static Quaternion FromMatrix(Matrix4x4 _mat);
};