#pragma once

#include "Matrix.h"
#include "../Quaternion/Quaternion.h"

namespace MatGenerateFunc
{
	// 位置から生成
	Matrix4x4 Translate(const Vector3& _vec);
	// 大きさから生成
	Matrix4x4 Scale(const Vector3& _scale);
	// 四元数から生成
	Matrix4x4 Rotate(const Quaternion& _rot);
	// X軸周りの回転から生成
	Matrix4x4 RotateX(float _rad);
	// Y軸周りの回転から生成
	Matrix4x4 RotateY(float _rad);
	// Z軸周りの回転から生成
	Matrix4x4 RotateZ(float _rad);
	// 位置、四元数、大きさから生成
	Matrix4x4 TRS(const Vector3& _pos, const Quaternion& _rot, const Vector3& _scale);
	// 位置、四元数、大きさから逆行列を生成
	Matrix4x4 InverseTRS(const Vector3& _pos, const Quaternion& _rot, const Vector3& _scale);
	/// <summary>
	/// 特定の位置から目標の位置を見る行列を作る
	/// </summary>
	/// <param name="_eye">特定の位置</param>
	/// <param name="_target">目標位置</param>
	/// <param name="_up">上</param>
	/// <returns></returns>
	Matrix4x4 LookAt(const Vector3& _eye, const Vector3& _target, const Vector3& _up);
}