#pragma once

#include "StorageAccessorsMacros.h"

#include "BodySlot.h"

#include "Rigid/RigidBodyStorage.h"

#include "Soft/RopeStorage.h"
#include "Soft/ClothStorage.h"
#include "Soft/SoftBodyStorage.h"



#include "SoftBodyUpdateInfo.h"

class BodyStorage
{
	// --- 剛体Body ---
	// 力
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(BodyID, Vector3, RigidBodyForce, rigidBodyStorage, Force);
	// 速度
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(BodyID, Vector3, RigidBodyVelocity, rigidBodyStorage, Velocity);
	// トルク
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(BodyID, Vector3, RigidBodyTorque, rigidBodyStorage, Torque);
	// 角速度
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(BodyID, Vector3, RigidBodyAngularVelocity, rigidBodyStorage, AngularVelocity);
	// 重力フラグ
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(BodyID, bool, RigidBodyIsGravity, rigidBodyStorage, IsGravity);
	// 重力加速度
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(BodyID, Vector3, RigidBodyGravity, rigidBodyStorage, Gravity);
	// 質量
	MD_OWNED_STORAGE_WRITE_ORIGINAL_ACCESSORS(BodyID, float, RigidBodyMass, rigidBodyStorage, Mass);
	// 質量の代入
	void SetRigidBodyMass(BodyID _id, float _mass);
	// 質量の逆数
	MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(BodyID, float, RigidBodyInverseMass, rigidBodyStorage, InverseMass);
	// ローカル慣性テンソルの逆数
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(BodyID, Matrix4x4, RigidBodyLocalInverseInertiaTensor, rigidBodyStorage, LocalInverseInertiaTensor);
	// 推定移動位置
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(BodyID, Vector3, RigidBodyPastPosition, rigidBodyStorage, PastPosition);
	// 推定姿勢
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(BodyID, Quaternion, RigidBodyPastRotation, rigidBodyStorage, PastRotation);
	// ローカル慣性テンソル変更フラグ
	MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(BodyID, bool, RigidBodyLocalInertiaDiary, rigidBodyStorage, LocalInertiaDiary);
	// 計算が完了したときに呼ぶ関数
	void LocalInertiaCalcSucces(BodyID _id);
	// ID
	MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(BodyID, BodyID, RigidBodyID, rigidBodyStorage, ID);
	// --- RopeBody ---
	// ID
	MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(BodyID, BodyID, RopeID, ropeStorage, ID);
	// GPU情報
	MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(BodyID, SolveGPUMeta, RopeMeta, ropeStorage, Meta);
	// 長さ
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(BodyID, float, RopeLength, ropeStorage, Length);
	// 切り分け数
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(BodyID, int, RopeSegmentCount, ropeStorage, SegmentCount);
	// --- ClothBody ---
	// ID
	MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(BodyID, BodyID, ClothID, clothStorage, ID);
	// GPU情報
	MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(BodyID, SolveGPUMeta, ClothMeta, clothStorage, Meta);
	// 横幅
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(BodyID, float, ClothWidth, clothStorage, Width);
	// 縦幅
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(BodyID, float, ClothHeight, clothStorage, Height);
	// 横切り分け数
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(BodyID, int, ClothColumnCount, clothStorage, ColumnCount);
	// 縦切り分け数
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(BodyID, int, ClothRowCount, clothStorage, RowCount);
	// --- SoftBody ---
	// ID
	MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(BodyID, BodyID, SoftBodyID, softBodyStorage, ID);
	// GPU情報
	MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(BodyID, SolveGPUMeta, SoftBodyMeta, softBodyStorage, Meta);
	// 横幅
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(BodyID, float, SoftBodyWidth, softBodyStorage, Width);
	// 縦幅
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(BodyID, float, SoftBodyHeight, softBodyStorage, Height);
	// 奥行き
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(BodyID, float, SoftBodyDepth, softBodyStorage, Depth);
	// 横切り分け数
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(BodyID, int, SoftBodySegmentCountX, softBodyStorage, SegmentCountX);
	// 縦切り分け数
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(BodyID, int, SoftBodySegmentCountY, softBodyStorage, SegmentCountY);
	// 奥行き分け数
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(BodyID, int, SoftBodySegmentCountZ, softBodyStorage, SegmentCountZ);
public:
	// コンストラクタ
	BodyStorage();

	// RigidBody作成
	BodyID CreateRigidBody(EntityID _entity, PhysicsTransformID _transformID);
	// Rope作成
	BodyID CreateRope(EntityID _entity, PhysicsTransformID _transformID, const RopeUpdateInfo& _info);
	// Cloth作成
	BodyID CreateCloth(EntityID _entity, PhysicsTransformID _transformID, const ClothUpdateInfo& _info);
	// SoftBody作成
	BodyID CreateSoftBody(EntityID _entity, PhysicsTransformID _transformID, const SoftBodyUpdateInfo& _info);

	// 破棄
	void Destroy(BodyID _id);

	// 生存確認
	bool IsAlive(BodyID _id) const;
	// 種類取得
	BodyType GetType(BodyID _id) const;
	// 実データのインデックス
	uint32_t GetDenseIndex(BodyID _id) const;
	// 持ってるEntity
	EntityID GetOwnerEntity(BodyID _id) const;
	// 対応Transform
	PhysicsTransformID GetTransformID(BodyID _id) const;
	// RigidBodyのBodyID取得
	bool TryGetRigidBodyID(PhysicsTransformID _transformID, BodyID& _output);
	// TransformIDと紐づくBodyIDがあるか否か
	bool Has(PhysicsTransformID _transformID) const;
private:
	// 一意なID発行関数
	BodyID GenerateBodyID(size_t _denseIndex, BodyType _type, EntityID _ownerEntity, PhysicsTransformID _transformID);
private:
	std::vector<BodySlot> slots;
	std::vector<uint32_t> freeSlots;

	// 各種ボディストレージ

	std::unique_ptr<RigidBodyStorage> rigidBodyStorage;

	std::unique_ptr<RopeStorage> ropeStorage;
	std::unique_ptr<ClothStorage> clothStorage;
	std::unique_ptr<SoftBodyStorage> softBodyStorage;

	// RigidBody限定対応MAP
	std::unordered_map<PhysicsTransformID, BodyID> transformMap;
};
