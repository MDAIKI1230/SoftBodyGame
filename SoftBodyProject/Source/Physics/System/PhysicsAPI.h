#pragma once

#include "MDMath.h"

#include "PhysicsWorld.h"

class PhysicsAPI
{
public:
	// --- Body系 ---

	// 追加可否判定
	static bool CanAddBody(EntityID _entity);

	// --- RigidBody ---

	// 作成
	static BodyID CreateRigidBody(EntityID _entity);

	// 力加算
	static void AddForce(BodyID _id, const Vector3& _force);
	// トルク加算
	static void AddTorque(BodyID _id, const Vector3& _torque);

	// 速度取得
	static const Vector3& GetVelocity(BodyID _id);
	// 速度変更
	static void SetVelocity(BodyID _id, const Vector3& _velocity);

	// 角速度取得
	static const Vector3& GetAngularVelocity(BodyID _id);
	// 角速度変更
	static void SetAngularVelocity(BodyID _id, const Vector3& _omega);

	// isGravity取得
	static bool GetIsGravity(BodyID _id);
	// isGravity変更
	static void SetIsGravity(BodyID _id, bool _isGravity);
	// 重力加速度取得
	static const Vector3& GetGravity(BodyID _id);
	// 重力加速度変更
	static void SetGravity(BodyID _id, const Vector3& _gravity);

	// 質量取得
	static float GetMass(BodyID _id);
	// 質量変更
	static void SetMass(BodyID _id, float _mass);

	// --- ソフト系 ---

	// ロープ作成
	static BodyID CreateRope(EntityID _entity, const RopeUpdateInfo _info);
	// クロス作成
	static BodyID CreateCloth(EntityID _entity, const ClothUpdateInfo _info);
	// ソフトボディ作成
	static BodyID CreateSoftBody(EntityID _entity, const SoftBodyUpdateInfo _info);

	// --- ロープ系 ---

	// 全体の長さ取得
	static float GetLength(BodyID _id);
	// 全体の長さ変更
	static void SetLength(BodyID _id, float _length);
	// 分割数取得
	static int GetSegmentCount(BodyID _id);
	// 分割数変更
	static void SetSegmentCount(BodyID _id, int _segmentCount);
	
	// --- クロス系 ---

	// 横幅取得
	static float GetClothWidth(BodyID _id);
	// 横幅変更
	static void SetClothWidth(BodyID _id, float _width);
	// 縦幅取得
	static float GetClothHeight(BodyID _id);
	// 縦幅変更
	static void SetClothHeight(BodyID _id, float _height);
	// 横分割取得
	static int GetRowCount(BodyID _id);
	// 横分割変更
	static void SetRowCount(BodyID _id, int _rowCount);
	// 縦分割取得
	static int GetColumnCount(BodyID _id);
	// 縦分割変更
	static void SetColumnCount(BodyID _id, int _columnCount);

	// --- ソフトボディ系 ---

	// 横幅取得
	static float GetSoftBodyWidth(BodyID _id);
	// 横幅変更
	static void SetSoftBodyWidth(BodyID _id, float _width);
	// 縦幅取得
	static float GetSoftBodyHeight(BodyID _id);
	// 縦幅変更
	static void SetSoftBodyHeight(BodyID _id, float _height);
	// 奥行き取得
	static float GetSoftBodyDepth(BodyID _id);
	// 奥行き変更
	static void SetSoftBodyDepth(BodyID _id, float _depth);
	// 横分割取得
	static int GetSegmentCountX(BodyID _id);
	// 横分割変更
	static void SetSegmentCountX(BodyID _id, int _segmentCountX);
	// 縦分割取得
	static int GetSegmentCountY(BodyID _id);
	// 縦分割変更
	static void SetSegmentCountY(BodyID _id, int _segmentCountY);
	// 奥行き分割取得
	static int GetSegmentCountZ(BodyID _id);
	// 奥行き分割変更
	static void SetSegmentCountZ(BodyID _id, int _segmentCountZ);

	// --- コライダー系 ---
	// --- コライダー生成 ---

	// 球作成
	static ColliderID CreateSphere(EntityID _entity, float _radius);
	// Box作成
	static ColliderID CreateBox(EntityID _entity, const Vector3& _scale);

	// --- 球コライダー ---

	// 半径取得
	static float GetRadius(ColliderID _id);
	// 半径変更
	static void SetRadius(ColliderID _id, float _radius);

	// --- 箱コライダー ---

	// X方向の長さ取得
	static float GetWidth(ColliderID _id);
	// X方向の長さ変更
	static void SetWidth(ColliderID _id, float _width);
	// Y方向の長さ取得
	static float GetHeight(ColliderID _id);
	// Y方向の長さ変更
	static void SetHeight(ColliderID _id, float _height);
	// Z方向の長さ取得
	static float GetDepth(ColliderID _id);
	// Z方向の長さ変更
	static void SetDepth(ColliderID _id, float _depth);

	// --- 拘束系 ---

	// 点拘束作成
	static ConstraintID CreatePointConstraint(EntityID _entity, const Vector3& _localOffset);

	// 距離拘束作成
	static ConstraintID CreateDistanceConstraint(EntityID _entity, const Vector3& _localOffset, float _distance);
	// 距離拘束の距離取得
	static float GetDistance(ConstraintID _id);
	// 距離拘束の距離設定
	static void SetDistance(ConstraintID _id, float _distance);

	// 拘束にEndPoint追加
	static void AddEndPoint(ConstraintID _id, EntityID _entity, const Vector3& _localOffset);
	// 拘束からEndPoint除外
	static void RemoveEndPoint(ConstraintID _id, EntityID _entity);

	static void SetWorld(PhysicsWorld* _physicsWorld)
	{
		physicsWorld = _physicsWorld;
		colliderStorage = _physicsWorld->GetColliderStorage();
		bodyStorage = _physicsWorld->GetRigidBodyStorage();
		transformStorage = _physicsWorld->GetPhysicsTransformStorage();
		constraintStorage = _physicsWorld->GetConstraintStorage();
	}
private:
	static PhysicsWorld* physicsWorld;
	static ColliderStorage* colliderStorage;
	static BodyStorage* bodyStorage;
	static PhysicsTransformStorage* transformStorage;
	static ConstraintStorage* constraintStorage;
};
