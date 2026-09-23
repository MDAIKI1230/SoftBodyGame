#pragma once

#include "StorageAccessorsMacros.h"

#include "DataStorageBase.h"

#include "ActiveRagdollID.h"
#include "RagdollID.h"
#include "SkeletonID.h"
#include "ModelHandle.h"

#include "Ragdoll.h"
#include "ActiveRagdoll.h"
#include "SkeletonData.h"

/*
	アクティブラグドールのストレージ
	構造体とかを用意するかはまだ決めかねる
*/

class ActiveRagdollStorage :public DataStorageBase<ActiveRagdollID>
{
public:
	// ラグドールのデータ
	MD_STORAGE_ID_READ_WRITE_COLUMN(ActiveRagdollID, RagdollID, RagdollID, ragdollIDs)
	// アクティブ用のデータ
	MD_STORAGE_ID_READ_WRITE_COLUMN(ActiveRagdollID, ActiveRagdoll, ActiveRagdoll, activeRagdolls)
	// 自身のID
	MD_STORAGE_ID_READ_ONLY_COLUMN(ActiveRagdollID, ActiveRagdollID, ID, ids)
	// Ownerは生成後に変えない
	MD_STORAGE_ID_READ_ONLY_COLUMN(ActiveRagdollID, EntityID, OwnerEntity, ownerEntities)
public:
	// 作成関数
	ActiveRagdollID Create(EntityID _entity, RagdollID _ragdollID, const Ragdoll& _ragdoll, const SkeletonData* _skeleton, const ActiveRagdollSetting& _setting);
	// 破棄
	void Destroy(ActiveRagdollID _id);

private:
	// ActiveRagdoll情報の作成関数
	bool CreateActiveRagdoll(EntityID _entity, const Ragdoll& _ragdoll, const SkeletonData* _skeleton, const ActiveRagdollSetting& _setting);
	// ActiveRagdoll情報の破棄関数
	void DestroyActiveRagdoll(RagdollID _ragdollID, const ActiveRagdoll& _activeRagdoll);
};