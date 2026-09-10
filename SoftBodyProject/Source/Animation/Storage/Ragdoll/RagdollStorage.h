#pragma once

#include "StorageAccessorsMacros.h"

#include "DataStorageBase.h"

#include "RagdollID.h"

#include "Ragdoll.h"
#include "RagdollBodyLink.h"

#include "ModelHandle.h"

/*
	Ragdollに関係する情報を扱い管理する
*/
class RagdollStorage :public DataStorageBase<RagdollID>
{
public:
	// ラグドールのデータ
	MD_STORAGE_ID_READ_WRITE_COLUMN(RagdollID, Ragdoll, Ragdoll, ragdolls)
	// ラグドールのデータ
	MD_STORAGE_ID_READ_WRITE_COLUMN(RagdollID, bool, NeedInitialize, neesInitializes)
	// 自身のID
	MD_STORAGE_ID_READ_ONLY_COLUMN(RagdollID, RagdollID, ID, ids)
	// Ownerは生成後に変えない
	MD_STORAGE_ID_READ_ONLY_COLUMN(RagdollID, EntityID, OwnerEntity, ownerEntities)
public:
	// 生成
	RagdollID Create(EntityID _entity, SkeletonID _skeletonID, ModelHandle _model);
	// 破棄
	void Destroy(RagdollID _id);
};
