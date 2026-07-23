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
	BodyID CreateRigidBody(const EntityID& _entity, const PhysicsTransformID& _transformID);
	// Rope作成
	BodyID CreateRope(const EntityID& _entity, const PhysicsTransformID& _transformID, const RopeUpdateInfo& _info);
	// Cloth作成
	BodyID CreateCloth(const EntityID& _entity, const PhysicsTransformID& _transformID, const ClothUpdateInfo& _info);
	// SoftBody作成
	BodyID CreateSoftBody(const EntityID& _entity, const PhysicsTransformID& _transformID, const SoftBodyUpdateInfo& _info);

	// 破棄
	void Destroy(const BodyID& _id);

	// 生存確認
	bool IsAlive(const BodyID& _id) const;
	// 種類取得
	BodyType GetType(const BodyID& _id) const;
	// 実データのインデックス
	uint32_t GetDenseIndex(const BodyID& _id) const;
	// 持ってるEntity
	EntityID GetOwnerEntity(const BodyID& _id) const;
	// 対応Transform
	PhysicsTransformID GetTransformID(const BodyID& _id) const;
	// RigidBodyのBodyID取得
	bool TryGetRigidBodyID(const PhysicsTransformID& _transformID, BodyID& _output);
	// TransformIDと紐づくBodyIDがあるか否か
	bool Has(const PhysicsTransformID& _transformID) const;
public:
	std::vector<BodySlot> slots;
	std::vector<uint32_t> freeSlots;

	std::unique_ptr<RigidBodyStorage> rigidBodyStorage;

	std::unique_ptr<RopeStorage> ropeStorage;
	std::unique_ptr<ClothStorage> clothStorage;
	std::unique_ptr<SoftBodyStorage> softBodyStorage;
private:
	// 一意なID発行関数
	BodyID GenerateBodyID(size_t _denseIndex, const BodyType& _type, const EntityID& _ownerEntity, const PhysicsTransformID& _transformID);
private:
	// RigidBody限定対応MAP
	std::unordered_map<PhysicsTransformID, BodyID> transformMap;
};
