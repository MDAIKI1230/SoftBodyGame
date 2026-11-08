#pragma once

#include "../Vector/Vector3/Vector3.h"
#include "../Quaternion/Quaternion.h"
#include "../Matrix/Matrix.h"

struct  Transform
{
public:
    // コンストラクタ
	Transform();
    // ローカル座標の前(z+方向)
    Vector3 Forward() const;
    // ローカル座標の右(x+方向)
    Vector3 Right() const;
    // ローカル座標の上(y+方向)
    Vector3 Up() const;

    // 位置更新系
    // ローカル座標の移動
    void TranslateLocal(const Vector3& _delta);
    // ワールド座標の移動
    void TranslateWorld(const Vector3& _delta);
    // 位置を設定
    void SetLocalPosition(const Vector3& _pos);

    // 回転更新系
    // 引数分回転
    void Rotate(const Quaternion& _rot);
    // オイラー角分回転
    void RotateEuler(const Vector3& _euler);
    /// <summary>
    /// 点と軸と角度で回転
    /// </summary>
    /// <param name="point">通る点</param>
    /// <param name="axis">軸</param>
    /// <param name="angle">角度(弧度法)</param>
    void RotateAround(const Vector3& _point, const Vector3& _axis, float _rad);
    // 四元数を設定
    void SetLocalRotation(const Quaternion& _rot);
    // 引数の位置(ワールド座標)を見る
    void LookAt(const Vector3& _target);

    // 大きさ更新系
    // 大きさを設定
    void SetLocalScale(const Vector3& _scale);
    // 各方向に大きさをかける
    void ScaleBy(const Vector3& _scale);

	// ワールド行列を設定
	void SetWorldMatrix(const Matrix4x4& _worldMat);

    // ローカル座標とワールド座標の変換系
    // 引数の位置をワールド座標にする。
    Vector3 TransformPoint(const Vector3& _localPoint) const;
    // 引数の位置をローカル座標にする。
    Vector3 InverseTransformPoint(const Vector3& _worldPoint) const;

    // 取得系
    // 位置
    const Vector3& GetPosition() const { return localPosition; }
    // 回転
    const Quaternion& GetRotation() const { return localRotation; }
    // 大きさ
    const Vector3& GetScale() const { return localScale; }
    // ローカル行列
    const Matrix4x4& GetLocalMatrix() const { return localMatrix; }
	// 位置
	const Vector3& GetWorldPosition() const { return worldPosition; }
	// 回転
	const Quaternion& GetWorldRotation() const { return worldRotation; }
	// 大きさ
	const Vector3& GetWorldScale() const { return worldScale; }
    // ワールド行列
    const Matrix4x4& GetWorldMatrix() const { return worldMatrix; }

    /// <summary>
    /// TRSに分解する関数
    /// </summary>
    /// <param name="_mat">元の行列</param>
    /// <param name="_pos">位置を入れる変数</param>
    /// <param name="_rot">四元数を入れる変数</param>
    /// <param name="_scale">大きさを入れる変数</param>
    static void DecomposeTRS(const Matrix4x4& _mat, Vector3& _pos, Quaternion& _rot, Vector3& _scale);

private:
    // ローカル位置
    Vector3 localPosition{ 0.0f,0.0f,0.0f };
    // ローカル回転
    Quaternion localRotation{ 0.0f,0.0f,0.0f,1.0f };
    // ローカル大きさ
    Vector3 localScale{ 1.0f,1.0f,1.0f };

    // ローカル行列
    Matrix4x4 localMatrix;

	// ワールド位置
	Vector3 worldPosition{ 0.0f,0.0f,0.0f };
	// ワールド回転
	Quaternion worldRotation{ 0.0f,0.0f,0.0f,1.0f };
	// ワールド大きさ
	Vector3 worldScale{ 1.0f,1.0f,1.0f };

    // ワールド行列
    Matrix4x4 worldMatrix;

private:
    // ローカル行列更新
    void UpdateLocalMatrix();
};
