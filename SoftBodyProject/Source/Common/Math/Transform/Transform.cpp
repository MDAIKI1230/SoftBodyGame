#include "../Vector/SIMD/SIMDVectorMath.h"
#include "../Matrix/MatGenerateFunc.h"
#include "Transform.h"

// ローカル座標の前(z+方向)
Vector3 Transform::Forward() const
{
	return rotation.Rotate(Vector3::FORWARD);
}
// ローカル座標の右(x+方向)
Vector3 Transform::Right() const
{
	return rotation.Rotate(Vector3::RIGHT);
}
// ローカル座標の上(y+方向)
Vector3 Transform::Up() const
{
	return rotation.Rotate(Vector3::UP);
}

// 位置更新系
// ローカル座標の移動
void Transform::TranslateLocal(const Vector3& _delta)
{
	position += rotation.Rotate(_delta);

	// 行列更新
	UpdateLocalMatrix();
}
// ワールド座標の移動
void Transform::TranslateWorld(const Vector3& _delta)
{
	position += _delta;

	// 行列更新
	UpdateLocalMatrix();
}
// 位置を設定
void Transform::SetPosition(const Vector3& _pos)
{
	position = _pos;

	// 行列更新
	UpdateLocalMatrix();
}

// 回転更新系
// 引数分回転
void Transform::Rotate(const Quaternion& _rot)
{
	rotation *= _rot;

	rotation.Normalize();
	// 行列更新
	UpdateLocalMatrix();
}
// オイラー角分回転
void Transform::RotateEuler(const Vector3& _euler)
{
	rotation *= Quaternion::Euler(_euler.x, _euler.y, _euler.z);

	// 行列更新
	UpdateLocalMatrix();
}
/// <summary>
/// 点と軸と角度で回転
/// </summary>
/// <param name="point">通る点</param>
/// <param name="axis">軸</param>
/// <param name="angle">角度(弧度法)</param>
void Transform::RotateAround(const Vector3& _point, const Vector3& _axis, float _rad)
{
	Quaternion q = Quaternion::AngleAxis(_rad, Vector3::Normalized(_axis));

	// 位置を回す
	Vector3 offset = position - _point;
	offset = q.Rotate(offset);
	position = _point + offset;

	// 向きも回す（必要なら）
	rotation = q * rotation;

	// 行列更新
	UpdateLocalMatrix();
}
// 四元数を設定
void Transform::SetRotation(const Quaternion& _rot)
{
	rotation = _rot;

	// 行列更新
	UpdateLocalMatrix();
}
// 引数の位置(ワールド座標)を見る
void Transform::LookAt(const Vector3& _target)
{
	rotation = Quaternion::LookAt(position, _target, Vector3::UP);

	// 行列更新
	UpdateLocalMatrix();
}

// 大きさ更新系
// 大きさを設定
void Transform::SetScale(const Vector3& _scale)
{
	scale = _scale;

	// 行列更新
	UpdateLocalMatrix();
}
// 各方向に大きさをかける
void Transform::ScaleBy(const Vector3& _scale)
{
	scale = SIMDVectorMath::Mul(scale, _scale);

	// 行列更新
	UpdateLocalMatrix();
}

// ローカル座標とワールド座標の変換系
// 引数の位置をワールド座標にする。
Vector3 Transform::TransformPoint(const Vector3& _localPoint) const
{
	return worldMatrix * _localPoint;
}
// 引数の位置をローカル座標にする。
Vector3 Transform::InverseTransformPoint(const Vector3& _worldPoint) const
{
	if (parent)
	{
		Vector3 p = parent->InverseTransformPoint(_worldPoint);

		Vector3 v = p - position;
		v = rotation.Conjugate().Rotate(v);
		v = SIMDVectorMath::Div(v, scale);
		return v;
	}
	else
	{
		Vector3 v = _worldPoint - position;
		v = rotation.Conjugate().Rotate(v);
		v = SIMDVectorMath::Div(v, scale);
		return v;
	}
}

// 親設定
void Transform::SetParent(Transform* _parent, bool keepLocal)
{
	if (keepLocal)
	{
		parent = _parent;
	}
	else
	{
		// 今のワールドを保存
		Matrix4x4 world = worldMatrix;

		parent = _parent;

		Matrix4x4 invParent;
		if (parent)
		{
			// 逆行列を生成
			invParent = MatGenerateFunc::InverseTRS(parent->GetPosition(), parent->GetRotate(), parent->GetScale());
		}
		else
		{
			// 親がないなら単位行列
			invParent = Matrix4x4::Identity();
		}
			
		// ローカルを作り直す
		localMatrix = invParent * world;

		// TRSに分解
		Transform::DecomposeTRS(localMatrix, position, rotation, scale);
	}

	// ワールド更新
	UpdateWorldMatrix();
}

// TRSに分解する関数
void Transform::DecomposeTRS(Matrix4x4& _mat, Vector3& _pos, Quaternion& _rot, Vector3& _scale)
{
	// 位置
	_pos = Vector3{ _mat.m[0][3],_mat.m[1][3],_mat.m[2][3] };

	// 四元数
	_rot = Quaternion::FromMatrix(_mat);

	float sx{ Vector3{_mat.m[0][0],_mat.m[1][0],_mat.m[2][0]}.Length() };
	float sy{ Vector3{_mat.m[0][1],_mat.m[1][1],_mat.m[2][1]}.Length() };
	float sz{ Vector3{_mat.m[0][2],_mat.m[1][2],_mat.m[2][2]}.Length() };

	// スケール
	_scale = Vector3{ sx,sy,sz };
}

// ローカル行列更新
void Transform::UpdateLocalMatrix()
{
	localMatrix = MatGenerateFunc::TRS(position, rotation, scale);

	UpdateWorldMatrix();
}
// ワールド行列更新
void Transform::UpdateWorldMatrix()
{
	if (parent)
	{
		worldMatrix = parent->GetWorldMatrix() * localMatrix;
	}
	else
	{
		worldMatrix = localMatrix;
	}
}