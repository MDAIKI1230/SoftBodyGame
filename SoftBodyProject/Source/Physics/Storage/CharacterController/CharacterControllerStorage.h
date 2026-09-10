#pragma once

#include <unordered_map>

#include "MDMath.h"

#include "StorageAccessorsMacros.h"

#include "DataStorageBase.h"

#include "CharacterControllerConstants.h"

#include "CharacterControllerID.h"
#include "PhysicsTransformID.h"
#include "ColliderID.h"
#include "BodyID.h"

class CharacterControllerStorage :public DataStorageBase<CharacterControllerID>
{
	// --- 操作要求 ---

	// 移動入力ベクトル
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, Vector3, MoveInput, moveInputs);
	// ジャンプリクエスト
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, bool, JumpRequest, jumpRequests);
	// 入力を受け付けるかフラグ
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, bool, MotorEnabled, motorEnableds);

	// --- 移動設定 ---
	
	// 最大速度(重力を除く入力からの最大速度)
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, float, MaxSpeed, maxSpeeds);
	// 地上にいる時の最大加速度
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, float, GroundAcceleration, groundAccelerations);
	// 地上にいる時の減速
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, float, GroundDeceleration, groundDecelerations);
	// 空中時の最大加速度
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, float, AirAcceleration, airAccelerations);
	// ジャンプの速度(強さ)
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, float, JumpSpeed, jumpSpeeds);
	// 滑り強める力
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, float, SlopeAcceleration, slopeAccelerations);

	// --- 接地設定 ---

	// 地面との距離
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, float, GroundProbeDistance, groundProbeDistances);
	// 最大移動できる地面の内積
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, float, MinGroundDot, minGroundDots);

	// --- 実行時状態 ---

	// 地面との接触状況
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, CharacterGroundState, GroundState, states);
	// 地面の法線
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, Vector3, GroundNormal, groundNormals);
	// 地面の位置
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, Vector3, GroundPoint, groundPoints);
	// 地面との距離
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, float, GroundDistance, groundDistances);
	// 地面のコライダーID
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, ColliderID, GroundColliderID, groundColliderIDs);

	// --- ID関連 ---

	// CharacterController自身のID
	MD_STORAGE_ID_READ_ONLY_COLUMN(CharacterControllerID, CharacterControllerID, ID, ids);
	// トランスフォーム
	MD_STORAGE_ID_READ_ONLY_COLUMN(CharacterControllerID, PhysicsTransformID, TransformID, transformIDs);
	// リジッドボディ
	MD_STORAGE_ID_READ_ONLY_COLUMN(CharacterControllerID, BodyID, RigidBodyID, rigidBodyIDs);
	// コライダー
	MD_STORAGE_ID_READ_ONLY_COLUMN(CharacterControllerID, ColliderID, CapsuleColliderID, capsuleColliderIDs);

	// 使用エンティティ
	MD_STORAGE_ID_READ_ONLY_COLUMN(CharacterControllerID, EntityID, OwnerEntity, ownerEntities)
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