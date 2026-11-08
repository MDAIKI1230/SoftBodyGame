#pragma once

#include "MDMath.h"

#include "EntityID.h"

struct TransformComponent
{
	// ラッパー関数のためヘッダに直書きしてる
	// のちにECS化する際に使うためComponentに入れてる
public:
	// デフォルトコンストラクタ
	TransformComponent() = default;

	// ローカル座標の前(z+方向)
	Vector3 Forward() const { return trans.Forward(); }
	// ローカル座標の右(x+方向)
	Vector3 Right() const { return trans.Right(); }
	// ローカル座標の上(y+方向)
	Vector3 Up() const { return trans.Up(); }

	// 位置更新系
	// ローカル座標の移動
	void TranslateLocal(const Vector3& _delta) { trans.TranslateLocal(_delta); isDirty = true; }
	// ワールド座標の移動
	void TranslateWorld(const Vector3& _delta) { trans.TranslateWorld(_delta); isDirty = true; }
	// 位置を設定
	void SetPosition(const Vector3& _pos) { trans.SetLocalPosition(_pos); isDirty = true; }

	// 回転更新系
	// 引数分回転
	void Rotate(const Quaternion& _rot) { trans.Rotate(_rot); isDirty = true; }
	// オイラー角分回転
	void RotateEuler(const Vector3& _euler) { trans.RotateEuler(_euler); isDirty = true; }
	/// <summary>
	/// 点と軸と角度で回転
	/// </summary>
	/// <param name="point">通る点</param>
	/// <param name="axis">軸</param>
	/// <param name="angle">角度(弧度法)</param>
	void RotateAround(const Vector3& _point, const Vector3& _axis, float _rad) { trans.RotateAround(_point, _axis, _rad); isDirty = true; }
	// 四元数を設定
	void SetRotation(const Quaternion& _rot) { trans.SetLocalRotation(_rot); isDirty = true; }
	// 引数の位置(ワールド座標)を見る
	void LookAt(const Vector3& _target) { trans.LookAt(_target); isDirty = true; }

	// 大きさ更新系
	// 大きさを設定
	void SetScale(const Vector3& _scale) { trans.SetLocalScale(_scale); isDirty = true; }
	// 各方向に大きさをかける
	void ScaleBy(const Vector3& _scale) { trans.ScaleBy(_scale); isDirty = true; }

	// ワールド行列を設定
	void SetWorldMatrix(const Matrix4x4& _worldMat) { trans.SetWorldMatrix(_worldMat); }

	// ローカル座標とワールド座標の変換系
	// 引数の位置をワールド座標にする。
	Vector3 TransformPoint(const Vector3& _localPoint) const { return trans.TransformPoint(_localPoint); }
	// 引数の位置をローカル座標にする。
	Vector3 InverseTransformPoint(const Vector3& _worldPoint) const { return trans.InverseTransformPoint(_worldPoint); }

	/// <summary>
	/// 親設定
	/// </summary>
	/// <param name="_parent">親のEntityID</param>
	void SetParent(EntityID _parent) { parent = _parent; isDirty = true; }

	// 取得系
	// 位置
	const Vector3& GetPosition()const { return trans.GetPosition(); }
	// 回転
	const Quaternion& GetRotation()const { return trans.GetRotation(); }
	// 大きさ
	const Vector3& GetScale()const { return trans.GetScale(); }
	// ローカル行列
	const Matrix4x4& GetLocalMatrix()const { return trans.GetLocalMatrix(); }
	// 位置
	const Vector3& GetWorldPosition() const { return trans.GetWorldPosition(); }
	// 回転
	const Quaternion& GetWorldRotation() const { return trans.GetWorldRotation(); }
	// 大きさ
	const Vector3& GetWorldScale() const { return trans.GetWorldScale(); }
	// ワールド行列
	const Matrix4x4& GetWorldMatrix()const { return trans.GetWorldMatrix(); }
	// 親
	const EntityID GetParent()const { return parent; }
	// 生データ取得
	Transform& GetData() { return trans; }
	// 変更の有無
	bool GetIsDirty() { return isDirty; }
	// 変更解消
	void EndReCalcMatrix() { isDirty = false; }
private:
	// トランスフォーム
	Transform trans{};
	// 変更フラグ
	bool isDirty{ false };
	// 親エンティティID
	EntityID parent;
};
