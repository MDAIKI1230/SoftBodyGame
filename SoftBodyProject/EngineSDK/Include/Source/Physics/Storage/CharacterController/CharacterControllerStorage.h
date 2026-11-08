#pragma once

#include <unordered_map>

#include "MDMath.h"

#include "StorageAccessorsMacros.h"

#include "DataStorageBase.h"

#include "CharacterControllerConstants.h"
#include "CollisionFilter.h"
#include "RayCastQueryHitInfo.h"

#include "EntityID.h"
#include "CharacterControllerID.h"
#include "PhysicsTransformID.h"
#include "BodyID.h"
#include "ConstraintID.h"

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
	// 最大駆動力の計算に使う斜面角度(度)
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, float, MaxMoverForceSlopeAngleDeg, maxMoverForceSlopeAngleDegrees);
	// 地上にいる時の加速度
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, float, GroundAcceleration, groundAccelerations);
	// 地上にいる時の減速
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, float, GroundDeceleration, groundDecelerations);
	// 落下時の最大速度
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, float, MaxFallSpeed, maxFallSpeeds);
	// 空中時の最大加速度
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, float, AirAcceleration, airAccelerations);
	// 空中にいる時の減速度
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, float, AirDeceleration, airDecelerations);
	// ジャンプの速度(強さ)
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, float, JumpSpeed, jumpSpeeds);
	// ジャンプ状態を保持する最大時間(秒)
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, float, MaxJumpTime, maxJumpTimes);
	// 空中時最大駆動力倍率
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, float, AirForceScale, airForceScales);
	// 空中時最大速度倍率
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, float, AirSpeedScale, airSpeedScales);
	// 滑り時最大駆動力倍率
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, float, SteepSlopeForceScale, steepSlopeForceScales);
	// 滑り時最大速度倍率
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, float, SteepSlopeSpeedScale, steepSlopeSpeedScales);

	// --- 接地設定 ---

	// 接地探索用のコリジョンフィルター
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, CollisionFilter, CollisionFilter, collisionFilters);
	// 地面探索距離・接地拘束で保つ支持距離
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, float, GroundProbeDistance, groundProbeDistances);
	// 最大移動できる地面の内積
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, float, MinGroundDot, minGroundDots);
	// スカートの半径
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, float, SkirtRadius, skirtRadii);
	// スカートの本数
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, int, SkirtRayCount, skirtRayCounts);
	// 採用レイの位置オフセット
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, Vector3, RayOffset, rayOffsets);

	// --- 実行時状態 ---

	// 地面との接触状況
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, CharacterGroundState, GroundState, states);
	// 現在のジャンプ経過時間(秒)
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, float, JumpTime, jumpTimes);
	// 接地に採用したレイのヒット情報
	MD_STORAGE_ID_READ_WRITE_COLUMN(CharacterControllerID, RayCastQueryHitInfo, RayInfo, rayInfos);
	// 地面との距離を保つ拘束ID
	MD_STORAGE_ID_READ_ONLY_COLUMN(CharacterControllerID, ConstraintID, GroundConstraintID, groundConstraintIDs);

	// --- ID関連 ---

	// CharacterController自身のID
	MD_STORAGE_ID_READ_ONLY_COLUMN(CharacterControllerID, CharacterControllerID, ID, ids);
	// トランスフォーム
	MD_STORAGE_ID_READ_ONLY_COLUMN(CharacterControllerID, PhysicsTransformID, TransformID, transformIDs);
	// リジッドボディ
	MD_STORAGE_ID_READ_ONLY_COLUMN(CharacterControllerID, BodyID, RigidBodyID, rigidBodyIDs);
	// 拘束用内部TransformID
	MD_STORAGE_ID_READ_ONLY_COLUMN(CharacterControllerID, PhysicsTransformID, InternalConstraintTransformID, internalConstraintTransformIDs)

	// 使用エンティティ
	MD_STORAGE_ID_READ_ONLY_COLUMN(CharacterControllerID, EntityID, OwnerEntity, ownerEntities)
public:
	// コンストラクタ
	CharacterControllerStorage() = default;

	// 作成
	CharacterControllerID Create(EntityID _entity, PhysicsTransformID _transformID, BodyID _rigidBodyID, ConstraintID _groundConstraintID, PhysicsTransformID _internalConstraintTransformID);

	// 破棄
	void Destroy(CharacterControllerID _id);

	// EntityがCharacterControllerを持っているか
	bool Has(EntityID _entity) const;
	// Bodyを使用しているCharacterControllerがあるか
	bool Has(BodyID _bodyID) const;

	// Entityに対応するCharacterControllerIDを取得
	bool TryGet(EntityID _entity, CharacterControllerID& _output) const;
	// Bodyに対応するCharacterControllerIDを取得
	bool TryGet(BodyID _bodyID, CharacterControllerID& _output) const;

private:
	std::unordered_map<EntityID, CharacterControllerID> entityMap;
	std::unordered_map<BodyID, CharacterControllerID> bodyMap;
};