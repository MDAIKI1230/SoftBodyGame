#pragma once

#include "StorageAccessorsMacros.h"

#include "DataStorageBase.h"

#include "RagdollID.h"
#include "ModelHandle.h"

#include "Ragdoll.h"
#include "SkeletonData.h"
#include "RagdollDefinition.h"

/*
	Ragdollに関係する情報を扱い管理する
*/
class RagdollStorage :public DataStorageBase<RagdollID>
{
public:
	// ラグドールのデータ
	MD_STORAGE_ID_READ_WRITE_COLUMN(RagdollID, Ragdoll, Ragdoll, ragdolls)
	// 初期化が必要かフラグ
	MD_STORAGE_ID_READ_WRITE_COLUMN(RagdollID, bool, NeedInitialize, neesInitializes)
	// 自身のID
	MD_STORAGE_ID_READ_ONLY_COLUMN(RagdollID, RagdollID, ID, ids)
	// Ownerは生成後に変えない
	MD_STORAGE_ID_READ_ONLY_COLUMN(RagdollID, EntityID, OwnerEntity, ownerEntities)
public:
	// 生成
	RagdollID Create(EntityID _entity, SkeletonID _skeletonID, ModelHandle _model, const SkeletonData* _skeleton, const std::string& _path);
	// 破棄
	void Destroy(RagdollID _id);

private:
	// Ragdoll情報の作成関数
	bool CreateRagdoll(EntityID _entity, SkeletonID _skeletonID, RagdollID _ragdollID, ModelHandle _model, const SkeletonData* _skeleton, const RagdollDefinition& _definition);

	// Ragdoll情報の破棄関数
	void DestroyRagdoll(const Ragdoll& _ragdoll);
};
