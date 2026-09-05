#pragma once

#include <unordered_map>

#include "MDMath.h"

#include "StorageAccessorsMacros.h"

#include "PhysicsStorageBase.h"

#include "CharacterControllerConstants.h"

#include "CharacterControllerID.h"
#include "PhysicsTransformID.h"
#include "ColliderID.h"
#include "BodyID.h"
#include "CharacterControllerSlot.h"

class CharacterControllerStorage :public PhysicsStorageBase<CharacterControllerID, CharacterControllerSlot>
{
	// --- 操作要求 ---

	// 移動入力ベクトル
	MD_STORAGE_READ_WRITE_COLUMN(Vector3, MoveInput, moveInputs);
	// ジャンプリクエスト
	MD_STORAGE_READ_WRITE_COLUMN(bool, JumpRequest, jumpRequests);
	// 入力を受け付けるかフラグ
	MD_STORAGE_READ_WRITE_COLUMN(bool, MotorEnabled, motorEnableds);

	// --- 移動設定 ---
	
	// 最大速度(重力を除く入力からの最大速度)
	MD_STORAGE_READ_WRITE_COLUMN(float, MaxSpeed, maxSpeeds);
	// 地上にいる時の最大加速度
	MD_STORAGE_READ_WRITE_COLUMN(float, GroundAcceleration, groundAccelerations);
	// 地上にいる時の減速
	MD_STORAGE_READ_WRITE_COLUMN(float, GroundDeceleration, groundDecelerations);
	// 空中時の最大加速度
	MD_STORAGE_READ_WRITE_COLUMN(float, AirAcceleration, airAccelerations);
	// ジャンプの速度(強さ)
	MD_STORAGE_READ_WRITE_COLUMN(float, JumpSpeed, jumpSpeeds);
	// 滑り強める力
	MD_STORAGE_READ_WRITE_COLUMN(float, SlopeAcceleration, slopeAccelerations);

	// --- 接地設定 ---

	// 地面との距離
	MD_STORAGE_READ_WRITE_COLUMN(float, GroundProbeDistance, groundProbeDistances);
	// 最大移動できる地面の内積
	MD_STORAGE_READ_WRITE_COLUMN(float, MinGroundDot, minGroundDots);

	// --- 実行時状態 ---

	// 地面との接触状況
	MD_STORAGE_READ_WRITE_COLUMN(CharacterGroundState, GroundState, states);
	// 地面の法線
	MD_STORAGE_READ_WRITE_COLUMN(Vector3, GroundNormal, groundNormals);
	// 地面の位置
	MD_STORAGE_READ_WRITE_COLUMN(Vector3, GroundPoint, groundPoints);
	// 地面との距離
	MD_STORAGE_READ_WRITE_COLUMN(float, GroundDistance, groundDistances);
	// 地面のコライダーID
	MD_STORAGE_READ_WRITE_COLUMN(ColliderID, GroundColliderID, groundColliderIDs);

	// --- ID関連 ---

	// CharacterController自身のID
	MD_STORAGE_READ_ONLY_COLUMN(CharacterControllerID, ID, ids);
	// トランスフォーム
	MD_STORAGE_READ_ONLY_COLUMN(PhysicsTransformID, TransformID, transformIDs);
	// リジッドボディ
	MD_STORAGE_READ_ONLY_COLUMN(BodyID, RigidBodyID, rigidBodyIDs);
	// コライダー
	MD_STORAGE_READ_ONLY_COLUMN(ColliderID, CapsuleColliderID, capsuleColliderIDs);

public:
	// コンストラクタ
	CharacterControllerStorage() = default;

	// 作成
	CharacterControllerID Create(EntityID _entity, PhysicsTransformID _transformID, BodyID _rigidBodyID, ColliderID _capsuleColliderID);

	// 破棄
	void Destroy(CharacterControllerID _id);

	// EntityがCharacterControllerを持っているか
	bool Has(EntityID _entity) const;

	// Entityに対応するCharacterControllerIDを取得
	bool TryGet(EntityID _entity, CharacterControllerID& _output) const;

private:
	std::unordered_map<EntityID, CharacterControllerID> entityMap;
};
