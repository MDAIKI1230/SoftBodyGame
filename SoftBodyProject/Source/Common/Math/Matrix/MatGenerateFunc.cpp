#include <bit>
#include "../Vector/SIMD/SIMDVectorMath.h"
#include "MatGenerateFunc.h"

Matrix4x4 MatGenerateFunc::Translate(const Vector3& _vec)
{
	return Matrix4x4{
		1.0f,0.0f,0.0f,_vec.x,
		0.0f,1.0f,0.0f,_vec.y,
		0.0f,0.0f,1.0f,_vec.z,
		0.0f,0.0f,0.0f,1.0f,
	};
}

Matrix4x4 MatGenerateFunc::Scale(const Vector3& _scale)
{
	return Matrix4x4
	{
		_scale.x,0.0f,0.0f,0.0f,
		0.0f,_scale.y,0.0f,0.0f,
		0.0f,0.0f,_scale.z,0.0f,
		0.0f,0.0f,0.0f,1.0f,
	};
}

Matrix4x4 MatGenerateFunc::Rotate(const Quaternion& _rot)
{
	// 2_rot
	SIMDVectorFloat rot2{ SIMDVectorMath::Add(_rot.simd, _rot.simd) };
	// 2_rot^2
	SIMDVectorFloat rotrot2{ SIMDVectorMath::Mul(_rot.simd, rot2) };

	// y^2 x^2 x^2 w^2
	SIMDVectorFloat v0{ SIMDVectorFloat::Shuffle<1,0,0,3>(rotrot2) };
	// z^2 z^2 y^2 w^2
	SIMDVectorFloat v1{ SIMDVectorFloat::Shuffle<2,2,1,3>(rotrot2) };

	SIMDVectorFloat one{ 1.0f };

	// 1 - v0 - v1
	SIMDVectorFloat elementDiagonal{ SIMDVectorMath::Sub(one,v0) };
	// 対角要素
	// 1 - 2y^2 - 2z^2, 1 - 2x^2 - 2z^2, 1 - x^2 - y^2
	elementDiagonal = SIMDVectorMath::Sub(elementDiagonal, v1);
	// XYZだけ残すようマスク
	SIMDVectorFloat maskXyz{
		std::bit_cast<float>(0xFFFFFFFFu),
		std::bit_cast<float>(0xFFFFFFFFu),
		std::bit_cast<float>(0xFFFFFFFFu),
		std::bit_cast<float>(0x00000000u) };

	// ｗを消す
	elementDiagonal = SIMDVectorMath::And(elementDiagonal, maskXyz);

	// wがない項を作る
	// x,x,y,w
	v0 = SIMDVectorFloat::Shuffle<0, 0, 1, 3>(_rot.simd);

	// 2y,2z,2z,2w
	v1 = SIMDVectorFloat::Shuffle<1, 2, 2, 3>(rot2);

	// 2xy,2xz,2yz,2w^2
	SIMDVectorFloat termXyz{ SIMDVectorMath::Mul(v0,v1) };

	// wの項を作る
	// 2xw,2yw,2zw,2w^2
	SIMDVectorFloat termW{ SIMDVectorMath::MulScalar(rot2,_rot.w) };
	// 2zw,2yw,2xw,2w^2
	termW = SIMDVectorFloat::Shuffle<2, 1, 0, 3>(termW);

	// 加算の要素
	// 2xy + 2zw, 2xz + 2yw, 2yz + 2xw
	SIMDVectorFloat elementAdd{ SIMDVectorMath::Add(termXyz,termW) };

	// 減算の要素
	// 2xy - 2zw, 2xz - 2yw, 2yz - 2xw
	SIMDVectorFloat elementSub{ SIMDVectorMath::Sub(termXyz,termW) };

	// ±でまとめる
	// 2xy + 2zw, 2yz + 2xw, 2xy - 2zw, 2xz - 2yw
	v0 = SIMDVectorFloat::Shuffle<0, 2, 0, 1>(elementAdd, elementSub);

	// 2xz + 2yw, 2xy + 2zw, 2yz - 2xw, 2xy - 2zw
	v1 = SIMDVectorFloat::Shuffle<1, 0, 2, 0>(elementAdd, elementSub);
	
	// 行ごとに要素を構築
	Matrix4x4 m;

	// 一行目(1 - 2y^2 - 2z^2, 2xy + 2zw, 2xz - 2yw)
	//  計算結果から値を抽出(1 - 2y^2 - 2x^2, 0, 2xy + 2zw, 2xz - 2yw)
	m.row[0] = SIMDVectorFloat::Shuffle<0, 3, 0, 3>(elementDiagonal, v0);

	// 整列(1 - 2y^2 - 2z^2, 2xy + 2wz, 2xz - 2yw)
	m.row[0] = SIMDVectorFloat::Shuffle<0, 2, 3, 1>(m.row[0]);

	// 二行目(2xy - 2zw, 1 - 2x^2 - 2z^2, 2yz + 2xw)
	// 計算結果から値を抽出(1 - 2x^2 - 2z^2, 0, 2xy - 2zw, 2yz + 2xw)
	m.row[1] = SIMDVectorFloat::Shuffle<1, 3, 2, 1>(elementDiagonal, v0);

	// 整列(2xy - 2zw, 1 - 2x^2 - 2z^2, 2yz + 2xw)
	m.row[1] = SIMDVectorFloat::Shuffle<2, 0, 3, 1>(m.row[1]);

	// 三行目(2xz + 2yw, 2yz - 2xw, 1 - 2x^2 - 2y^2)
	// 計算結果から値を抽出(1 - x^2 - y^2, 0, 2yz - 2xw, 2xz + 2yw)
	m.row[2] = SIMDVectorFloat::Shuffle<2, 3, 2, 0>(elementDiagonal, v1);

	// 整列(2xz + 2yw, 2yz - 2xw, 1 - 2x^2 - 2y^2)
	m.row[2] = SIMDVectorFloat::Shuffle<3, 2, 0, 1>(m.row[2]);

	// 四行目は情報なし(０)
	m.row[3] = SIMDVectorFloat{ 0.0f,0.0f,0.0f,1.0f };

	return m;
}

Matrix4x4 MatGenerateFunc::RotateX(float _rad)
{
	float c{ cosf(_rad) };
	float s{ sinf(_rad) };

	return
	{
		1,0,0,0,
		0,c,-s,0,
		0,s,c,0,
		0,0,0,1
	};
}

Matrix4x4 MatGenerateFunc::RotateY(float _rad)
{
	float c{ cosf(_rad) };
	float s{ sinf(_rad) };

	return
	{
		c,0,s,0,
		0,1,0,0,
		-s,0,c,0,
		0,0,0,1
	};
}

Matrix4x4 MatGenerateFunc::RotateZ(float _rad)
{
	float c{ cosf(_rad) };
	float s{ sinf(_rad) };

	return
	{
		c,-s,0,0,
		s,c,0,0,
		0,0,1,0,
		0,0,0,1
	};
}

Matrix4x4 MatGenerateFunc::TRS(const Vector3& _pos, const Quaternion& _rot, const Vector3& _scale)
{
	return Translate(_pos) * Rotate(_rot) * Scale(_scale);
}

Matrix4x4 MatGenerateFunc::InverseTRS(const Vector3& _pos, const Quaternion& _rot, const Vector3& _scale)
{
	Vector3 invScale = SIMDVectorMath::Reciprocal(_scale);
	Quaternion invRot = _rot.Conjugate();

	Vector3 invPos = invRot.Rotate(-Vector3{ SIMDVectorMath::Mul(_pos, invScale) });

	return MatGenerateFunc::TRS(invPos, invRot, invScale);
}

Matrix4x4 MatGenerateFunc::LookAt(const Vector3& _eye, const Vector3& _target, const Vector3& _up)
{
	// 左手系
	Vector3 frwd{ (_target - _eye).Normalize()};
	Vector3 right{ Vector3::Cross(frwd,_up).Normalize() };
	Vector3 up{ Vector3::Cross(right,frwd) };

	// 右との内積
	float dotR{ Vector3::Dot(right,_eye) };
	// 上との内積
	float dotU{ Vector3::Dot(up,_eye) };
	// 前との内積
	float dotF{ Vector3::Dot(frwd,_eye) };

	// 列ベクトル
	return
	{
		right.x,up.x,frwd.x,0.0f,
		right.y,up.y,frwd.y,0.0f,
		right.z,up.z,frwd.z,0.0f,
		-dotR,-dotU,-dotF,1.0f
	};
}