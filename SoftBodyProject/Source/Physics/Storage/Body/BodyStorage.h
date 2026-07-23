#pragma once

#include <memory>

#include "BodySlot.h"

#include "Rigid/RigidBodyStorage.h"

#include "Soft/RopeStorage.h"
#include "Soft/ClothStorage.h"
#include "Soft/SoftBodyStorage.h"

#include "RopeUpdateInfo.h"
#include "ClothUpdateInfo.h"
#include "SoftBodyUpdateInfo.h"

class BodyStorage
{
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
public:
	std::vector<BodySlot> slots;
	std::vector<uint32_t> freeSlots;

	std::unique_ptr<RigidBodyStorage> rigidBodyStorage;

	std::unique_ptr<RopeStorage> ropeStorage;
	std::unique_ptr<ClothStorage> clothStorage;
	std::unique_ptr<SoftBodyStorage> softBodyStorage;
private:
	// 一意なID発行関数
	BodyID GenerateBodyID(size_t _denseIndex, BodyType _type, EntityID _ownerEntity, PhysicsTransformID _transformID);
private:
	// RigidBody限定対応MAP
	std::unordered_map<PhysicsTransformID, BodyID> transformMap;
};
