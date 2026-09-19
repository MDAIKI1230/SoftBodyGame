#pragma once

#include <unordered_map>

#include "StorageAccessorsMacros.h"

#include "DataStorageBase.h"

#include "SkeletonID.h"
#include "ModelHandle.h"

#include "SkeletonInstanceData.h"

/*
	モデルのインスタンスごとのスケルトンを扱うストレージ
	これは、すべてのアニメーションの基礎となる部分
*/
class SkeletonInstanceStorage :public DataStorageBase<SkeletonID>
{
public:
	// スケルトンのインスタンスデータ
	MD_STORAGE_ID_READ_WRITE_COLUMN(SkeletonID, SkeletonInstanceData, SkeletonInstanceData, skeletonInstanceDatas)
	// モデルの姿勢をワールド姿勢に変換する行列
	MD_STORAGE_ID_READ_WRITE_COLUMN(SkeletonID, Matrix4x4, WorldFromModel, worldFromModels)
	// 自身のID
	MD_STORAGE_ID_READ_ONLY_COLUMN(SkeletonID, SkeletonID, ID, ids)
	// Ownerは生成後に変えない
	MD_STORAGE_ID_READ_ONLY_COLUMN(SkeletonID, EntityID, OwnerEntity, ownerEntities)
public:
	// 作成
	SkeletonID Create(EntityID _entity, ModelHandle _model);
	// 破棄
	void Destroy(SkeletonID _id);

	// エンティティと対応したSkeletonIDがあるならそれを返すないなら作って返す
	SkeletonID CreateOrGetID(EntityID _entity, ModelHandle _model);
private:
	std::unordered_map<EntityID, SkeletonID> entityLookUp;
};
