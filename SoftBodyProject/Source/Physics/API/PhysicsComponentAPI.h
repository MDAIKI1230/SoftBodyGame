#pragma once

#include <span>

#include "MDMath.h"

#include "PhysicsWorld.h"

#include "TransformComponent.h"
#include "ColliderComponent.h"

#include "WorldStorage.h"

/*
	コンポーネントの内部用のAPI
	各種コンポーネントが実データを変更する時などに使う
	物理系のコンポーネントだけでなく他のコンポーネントの内部で使ってもよい
*/
class PhysicsComponentAPI
{
public:
	// --- Body系 ---

	// 追加可否判定
	static bool CanAddBody(EntityID _entity);
	// 破壊
	static void DestroyBody(BodyID _id);

	// --- RigidBody ---

	// 作成
	static BodyID CreateRigidBody(EntityID _entity);

	// 位置取得
	static Vector3 GetRigidBodyPosition(BodyID _id);
	// 位置変更
	static void SetRigidBodyPosition(BodyID _id, const Vector3& _position);

	// 回転取得
	static Quaternion GetRigidBodyRoatation(BodyID _id);
	// 回転変更
	static void SetRigidBodyRoatation(BodyID _id, const Quaternion& _rotation);

	// スケール取得
	static Vector3 GetRigidBodyScale(BodyID _id);
	// スケール変更
	static void SetRigidBodyScale(BodyID _id, const Vector3& _scale);

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

	// 回転制限取得
	static RigidBodyRotationLock GetRotationLock(BodyID _id);
	// 回転制限変更
	static void SetRotationLock(BodyID _id, RigidBodyRotationLock _lock);

	// 移動制限取得
	static RigidBodyPositionLock GetPositionLock(BodyID _id);
	// 移動制限変更
	static void SetPositionLock(BodyID _id, RigidBodyPositionLock _lock);

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
	// 破壊
	static void DestroyCollider(ColliderID _id);
	// --- コライダー生成 ---

	// 球作成
	static ColliderID CreateSphere(EntityID _entity, float _radius);
	// ボックス作成
	static ColliderID CreateBox(EntityID _entity, const Vector3& _scale);
	// カプセル作成
	static ColliderID CreateCapsule(EntityID _entity, float _height, float _radius);

	// --- 球コライダー ---

	// 半径取得
	static float GetSphereRadius(ColliderID _id);
	// 半径変更
	static void SetSphereRadius(ColliderID _id, float _radius);
	// オフセット位置取得
	static Vector3 GetSphereOffsetPosition(ColliderID _id);
	// オフセット位置変更
	static void SetSphereOffsetPosition(ColliderID _id, const Vector3& _offset);
	// オフセット回転取得
	static Quaternion GetSphereOffsetRotation(ColliderID _id);
	// オフセット回転変更
	static void SetSphereOffsetRotation(ColliderID _id, const Quaternion& _offset);

	// --- 箱コライダー ---

	// X方向の長さ取得
	static float GetBoxWidth(ColliderID _id);
	// X方向の長さ変更
	static void SetBoxWidth(ColliderID _id, float _width);
	// Y方向の長さ取得
	static float GetBoxHeight(ColliderID _id);
	// Y方向の長さ変更
	static void SetBoxHeight(ColliderID _id, float _height);
	// Z方向の長さ取得
	static float GetBoxDepth(ColliderID _id);
	// Z方向の長さ変更
	static void SetBoxDepth(ColliderID _id, float _depth);
	// オフセット位置取得
	static Vector3 GetBoxOffsetPosition(ColliderID _id);
	// オフセット位置変更
	static void SetBoxOffsetPosition(ColliderID _id, const Vector3& _offset);
	// オフセット回転取得
	static Quaternion GetBoxOffsetRotation(ColliderID _id);
	// オフセット回転変更
	static void SetBoxOffsetRotation(ColliderID _id, const Quaternion& _offset);

	// --- カプセルコライダー ---

	// 高さ取得
	static float GetCapsuleHeight(ColliderID _id);
	// 高さ変更
	static void SetCapsuleHeight(ColliderID _id, float _height);
	// 半径取得
	static float GetCapsuleRadius(ColliderID _id);
	// 半径変更
	static void SetCapsuleRadius(ColliderID _id, float _radius);
	// オフセット位置取得
	static Vector3 GetCapsuleOffsetPosition(ColliderID _id);
	// オフセット位置変更
	static void SetCapsuleOffsetPosition(ColliderID _id, const Vector3& _offset);
	// オフセット回転取得
	static Quaternion GetCapsuleOffsetRotation(ColliderID _id);
	// オフセット回転変更
	static void SetCapsuleOffsetRotation(ColliderID _id, const Quaternion& _offset);

	// フィルター取得
	static const CollisionFilter& GetFilter(ColliderID _id);
	// フィルター変更
	static void SetFilter(ColliderID _id, const CollisionFilter& _filter);

	// サポート関数
	static Vector3 Support(ColliderID _id, const Vector3& _dir);

	// --- 拘束系 ---
	// 破壊
	static void DestroyConstraint(ConstraintID _id);

	// 点拘束作成
	static ConstraintID CreatePointConstraint(EntityID _entity, const Vector3& _localOffset);
	// 距離拘束作成
	static ConstraintID CreateDistanceConstraint(EntityID _entity, const Vector3& _localOffset);

	// 距離拘束の距離取得
	static float GetDistance(ConstraintID _id);
	// 距離拘束の距離設定
	static void SetDistance(ConstraintID _id, float _distance);

	// ヒンジ拘束作成
	static ConstraintID CreateHingeConstraint(EntityID _entity, const Vector3& _localOffset, const Quaternion& _localRotation);
	// 角度制限付き点拘束作成
	static ConstraintID CreateAngleLimitPointConstraint(EntityID _entity, const Vector3& _localOffset, const Quaternion& _localRotation);
	// 角度制限付きヒンジ拘束作成
	static ConstraintID CreateAngleLimitHingeConstraint(EntityID _entity, const Vector3& _localOffset, const Quaternion& _localRotation);
	// SwingTwist拘束作成関数
	static ConstraintID CreateLimitedBallJointConstraint(EntityID _entity, const Vector3& _localOffset, const Quaternion& _localRotation);
	// 関節駆動拘束作成関数
	static ConstraintID CreateJointDriveConstraint(EntityID _entity, const Vector3& _localOffset, const Quaternion& _localRotation);

	// 自信のEndPoint取得
	static const EndPointFrame& GetEndPoint(ConstraintID _id);
	// 自信のEndPoint変更
	static void SetEndPoint(ConstraintID _id,const EndPointFrame& _endPoint);
	// 相手のEndPointすべて取得
	static std::span<const EndPointFrame> GetOtherEndPoints(ConstraintID _id);

	// 拘束にEndPoint追加
	static void AddEndPoint(ConstraintID _id, EntityID _entity, const Vector3& _localOffset, const Quaternion& _localRotation = Quaternion::IDENTITY);
	// 拘束からEndPoint除外
	static void RemoveEndPoint(ConstraintID _id, EntityID _entity);

	// Swing角度取得
	static float GetSwingAngle(ConstraintID _id);
	// Swing角度変更
	static void SetSwingAngle(ConstraintID _id, float _angle);
	// Twist角度最大取得
	static float GetTwistAngleMax(ConstraintID _id);
	// Twist角度最大変更
	static void SetTwistAngleMax(ConstraintID _id, float _angle);
	// Twist角度最小取得
	static float GetTwistAngleMin(ConstraintID _id);
	// Twist角度最小変更
	static void SetTwistAngleMin(ConstraintID _id, float _angle);

	// 最小角度取得
	static float GetAngleMin(ConstraintID _id);
	// 最小角度変更
	static void SetAngleMin(ConstraintID _id, float _angleMin);
	// 最大角度取得
	static float GetAngleMax(ConstraintID _id);
	// 最大角度変更
	static void SetAngleMax(ConstraintID _id, float _angleMax);
	// 角度範囲変更
	static void SetAngleRange(ConstraintID _id, float _angleMin, float _angleMax);

	// 関節駆動拘束の相対姿勢取得
	static Quaternion GetTargetRelativeRotation(ConstraintID _id);
	// 関節駆動拘束の相対姿勢変更
	static void SetTargetRelativeRotation(ConstraintID _id,const Quaternion& _targetRelativeRotation);

	// 単一Tuning取得
	static ConstraintTuning GetTuning(ConstraintID _id);
	// 単一Tuning変更
	static void SetTuning(ConstraintID _id, const ConstraintTuning& _tuning);

	// 位置Tuning取得
	static ConstraintTuning GetPositionTuning(ConstraintID _id);
	// 位置Tuning変更
	static void SetPositionTuning(ConstraintID _id, const ConstraintTuning& _tuning);

	// 回転Tuning取得
	static ConstraintTuning GetAngularTuning(ConstraintID _id);
	// 回転Tuning変更
	static void SetAngularTuning(ConstraintID _id, const ConstraintTuning& _tuning);

	// --- キャラクターコントローラー ---

	// 作成
	static CharacterControllerID CreateCharacterController(EntityID _entity);
	// 破壊
	static void DestroyCharacterController(CharacterControllerID _id);
	// 追加可否判定
	static bool CanAddCharacterController(EntityID _entity);

	// --- 回転 ---

	// 現在の回転取得
	static Quaternion GetCharacterControllerRotation(CharacterControllerID _id);
	// 回転設定
	static void SetCharacterControllerRotation(CharacterControllerID _id, const Quaternion& _rotation);
	// 現在の回転へ加算
	static void RotateCharacterController(CharacterControllerID _id, const Quaternion& _rotation);

	// --- 操作要求 ---

	// ワールド空間の移動入力取得
	static const Vector3& GetCharacterControllerMoveInput(CharacterControllerID _id);
	// ワールド空間の移動入力設定
	static void SetCharacterControllerMoveInput(CharacterControllerID _id, const Vector3& _moveInput);

	// ジャンプ要求取得
	static bool GetCharacterControllerJumpRequest(CharacterControllerID _id);
	// ジャンプ要求設定
	static void SetCharacterControllerJumpRequest(CharacterControllerID _id, bool _request);

	// 標準移動制御の有効状態取得
	static bool GetCharacterControllerMotorEnabled(CharacterControllerID _id);
	// 標準移動制御の有効状態設定
	static void SetCharacterControllerMotorEnabled(CharacterControllerID _id, bool _enabled);

	// --- Collider設定 ---

	// カプセル中央部分の高さ取得
	static float GetCharacterControllerColliderHeight(CharacterControllerID _id);
	// カプセル中央部分の高さ設定
	static void SetCharacterControllerColliderHeight(CharacterControllerID _id, float _height);

	// カプセル半径取得
	static float GetCharacterControllerColliderRadius(CharacterControllerID _id);
	// カプセル半径設定
	static void SetCharacterControllerColliderRadius(CharacterControllerID _id, float _radius);

	// --- 移動設定 ---

	// 最大移動速度取得
	static float GetCharacterControllerMaxSpeed(CharacterControllerID _id);
	// 最大移動速度設定
	static void SetCharacterControllerMaxSpeed(CharacterControllerID _id, float _maxSpeed);

	// 地上での最大加速度取得
	static float GetCharacterControllerGroundAcceleration(CharacterControllerID _id);
	// 地上での最大加速度設定
	static void SetCharacterControllerGroundAcceleration(CharacterControllerID _id, float _acceleration);

	// 地上での最大減速度取得
	static float GetCharacterControllerGroundDeceleration(CharacterControllerID _id);
	// 地上での最大減速度設定
	static void SetCharacterControllerGroundDeceleration(CharacterControllerID _id, float _deceleration);

	// 空中での最大加速度取得
	static float GetCharacterControllerAirAcceleration(CharacterControllerID _id);
	// 空中での最大加速度設定
	static void SetCharacterControllerAirAcceleration(CharacterControllerID _id, float _acceleration);

	// ジャンプ速度取得
	static float GetCharacterControllerJumpSpeed(CharacterControllerID _id);
	// ジャンプ速度設定
	static void SetCharacterControllerJumpSpeed(CharacterControllerID _id, float _jumpSpeed);

	// 急斜面での滑り加速度取得
	static float GetCharacterControllerSlopeAcceleration(CharacterControllerID _id);
	// 急斜面での滑り加速度設定
	static void SetCharacterControllerSlopeAcceleration(CharacterControllerID _id, float _acceleration);

	// --- 接地設定 ---

	// 地面探索距離取得
	static float GetCharacterControllerGroundProbeDistance(CharacterControllerID _id);
	// 地面探索距離設定
	static void SetCharacterControllerGroundProbeDistance(CharacterControllerID _id, float _distance);

	// 歩行可能判定の最小法線内積取得
	static float GetCharacterControllerMinGroundDot(CharacterControllerID _id);
	// 歩行可能判定の最小法線内積設定
	static void SetCharacterControllerMinGroundDot(CharacterControllerID _id, float _minGroundDot);

	// --- 接地状態 ---

	// 接地状態取得
	static CharacterGroundState GetCharacterControllerGroundState(CharacterControllerID _id);
	// 検出した地面法線取得
	static const Vector3& GetCharacterControllerGroundNormal(CharacterControllerID _id);
	// 検出した地面位置取得
	static const Vector3& GetCharacterControllerGroundPoint(CharacterControllerID _id);
	// カプセル底面から地面までの距離取得
	static float GetCharacterControllerGroundDistance(CharacterControllerID _id);
	// 検出した地面Collider取得
	static ColliderComponent GetCharacterControllerGroundCollider(CharacterControllerID _id);

	// --- 内部計算用関数 ---
	
	// 内部用のPhysicsTransform作成(寿命管理をちゃんを忘れない)
	static PhysicsTransformID CreateInternalPhysicsTransformID(EntityID _entity, const Vector3& _position, const Quaternion& _rotation, const Vector3& _scale);
	// 内部用のRigidBody作成(寿命管理をちゃんを忘れない)
	static BodyID CreateInternalRigidBody(EntityID _entity, PhysicsTransformID _transformID);
	// 内部用の球コライダー作成(寿命管理をちゃんを忘れない)
	static ColliderID CreateInternalSphereCollider(EntityID _entity, PhysicsTransformID _transformID, float _radius);
	// 内部用のカプセルコライダー作成(寿命管理をちゃんを忘れない)
	static ColliderID CreateInternalCapsuleCollider(EntityID _entity, PhysicsTransformID _transformID, float _height, float _radius);
	// 内部用のボックスコライダー作成(寿命管理をちゃんを忘れない)
	static ColliderID CreateInternalBoxCollider(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _scale);

	// 内部用の点拘束作成(寿命管理をちゃんを忘れない)
	static ConstraintID CreateInternalPointConstraint(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _localOffset);
	// 内部用の距離拘束作成(寿命管理をちゃんを忘れない)
	static ConstraintID CreateInternalDistanceConstraint(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _localOffset);
	// 内部用のヒンジ拘束作成(寿命管理をちゃんを忘れない)
	static ConstraintID CreateInternalHingeConstraint(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _localOffset, const Quaternion& _localRotation);
	// 内部用の角度制限付き点拘束作成(寿命管理をちゃんを忘れない)
	static ConstraintID CreateInternalAngleLimitPointConstraint(EntityID _entity, PhysicsTransformID _transformID, const Vector3& _localOffset, const Quaternion& _localRotation);
	// 内部用の角度制限付きヒンジ拘束作成(寿命管理をちゃんを忘れない)
	static ConstraintID CreateInternalAngleLimitHingeConstraint(
		EntityID _entity, PhysicsTransformID _transformID,
		const Vector3& _localOffset, const Quaternion& _localRotation);
	// SwingTwist拘束作成関数(寿命管理をちゃんを忘れない)
	static ConstraintID CreateInternalLimitedBallJointConstraint(
		EntityID _entity, PhysicsTransformID _transformID,
		const Vector3& _localOffset, const Quaternion& _localRotation);
	// 関節駆動拘束作成関数
	static ConstraintID CreateInternalJointDriveConstraint(
		EntityID _entity, PhysicsTransformID _transformID,
		const Vector3& _localOffset, const Quaternion& _localRotation);

	// 内部用拘束のEndPoint追加(寿命管理をちゃんを忘れない)
	static void AddInternalEndPoint(ConstraintID _constraintID, PhysicsTransformID _transformID, const Vector3& _localOffset, const Quaternion& _localRotation = Quaternion::IDENTITY);

	// PhysicsTransform破棄(対応する他の奴も破棄する)
	static void DestroyPhysicsTransform(PhysicsTransformID _transformID);

	static void BindWorld(PhysicsWorld& _physicsWorld, WorldStorage& _componentWorld);
	static void UnbindWorld();

private:
	friend struct ColliderComponent;

	// ColliderComponent内部用
	static bool IsColliderAlive(ColliderID _id);
	static ColliderType GetColliderType(ColliderID _id);

	// CharacterController内部情報取得
	static ColliderID GetCharacterControllerColliderID(CharacterControllerID _id);
	static TransformComponent* TryGetCharacterControllerTransform(CharacterControllerID _id);

	// ColliderのAABB変更通知
	static void MarkColliderDirty(ColliderID _id, AABBChangeDiaryFlag _flag);

	static WorldStorage* componentWorld;
	static PhysicsWorld* physicsWorld;
	static ColliderStorage* colliderStorage;
	static BodyStorage* bodyStorage;
	static PhysicsTransformStorage* transformStorage;
	static ConstraintStorage* constraintStorage;
	static CharacterControllerStorage* characterControllerStorage;
	static PhysicsCommandBuffer* commandBuffer;

	PhysicsComponentAPI();
};
