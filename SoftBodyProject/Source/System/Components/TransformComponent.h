#pragma once

#include "MDMath.h"

struct TransformComponent
{
// ラッパー関数のためヘッダに直書きしてる
// のちにECS化する際に使うためComponentに入れてる
public:
	TransformComponent() = default;
	
    // ローカル座標の前(z+方向)
    Vector3 Forward() const { trans.Forward(); }
    // ローカル座標の右(x+方向)
    Vector3 Right() const { trans.Right(); }
    // ローカル座標の上(y+方向)
    Vector3 Up() const { trans.Up(); }

    // 位置更新系
    // ローカル座標の移動
    void TranslateLocal(const Vector3& _delta) { trans.TranslateLocal(_delta); }
    // ワールド座標の移動
    void TranslateWorld(const Vector3& _delta) { trans.TranslateWorld(_delta); }
    // 位置を設定
    void SetPosition(const Vector3& _pos) { trans.SetPosition(_pos); }

    // 回転更新系
    // 引数分回転
    void Rotate(const Quaternion& _rot) { trans.Rotate(_rot); }
    // オイラー角分回転
    void RotateEuler(const Vector3& _euler) { trans.RotateEuler(_euler); }
    /// <summary>
    /// 点と軸と角度で回転
    /// </summary>
    /// <param name="point">通る点</param>
    /// <param name="axis">軸</param>
    /// <param name="angle">角度(弧度法)</param>
    void RotateAround(const Vector3& _point, const Vector3& _axis, float _rad) { trans.RotateAround(_point, _axis, _rad); }
    // 四元数を設定
    void SetRotation(const Quaternion& _rot) { trans.SetRotation(_rot); }
    // 引数の位置(ワールド座標)を見る
    void LookAt(const Vector3& _target) { trans.LookAt(_target); }

    // 大きさ更新系
    // 大きさを設定
    void SetScale(const Vector3& _scale) { trans.SetScale(_scale); }
    // 各方向に大きさをかける
    void ScaleBy(const Vector3& _scale) { trans.ScaleBy(_scale); }

    // ローカル座標とワールド座標の変換系
    // 引数の位置をワールド座標にする。
    Vector3 TransformPoint(const Vector3& _localPoint) const { trans.TransformPoint(_localPoint); }
    // 引数の位置をローカル座標にする。
    Vector3 InverseTransformPoint(const Vector3& _worldPoint) const { trans.InverseTransformPoint(_worldPoint); }

    /// <summary>
    /// 親設定
    /// </summary>
    /// <param name="_parent">親のTransform</param>
    /// <param name="keepLocal">ローカル行列を維持するか</param>
    void SetParent(Transform* _parent, bool keepLocal = true) { trans.SetParent(_parent, keepLocal); }

    // 取得系
    // 位置
    const Vector3& GetPosition()const { return trans.GetPosition(); }
    // 回転
    const Quaternion& GetRotate()const { return trans.GetRotate(); }
    // 大きさ
    const Vector3& GetScale()const { return trans.GetScale(); }
    // ローカル行列
    const Matrix4x4& GetLocalMatrix()const { return trans.GetLocalMatrix(); }
    // ワールド行列
    const Matrix4x4& GetWorldMatrix()const { return trans.GetWorldMatrix(); }
    // 親
    const Transform* GetParent()const { return trans.GetParent(); }
private:
	// トランスフォーム
	Transform trans{};
};