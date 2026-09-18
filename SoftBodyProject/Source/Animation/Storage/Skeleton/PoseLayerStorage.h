#pragma once

#include "StorageAccessorsMacros.h"

#include "DataStorageBase.h"

#include "PoseLayerID.h"
#include "SkeletonID.h"

#include "PoseLayer.h"
#include "Buffer/PoseBuffer.h"

/*
	ポーズレイヤーのストレージ
	各アニメーションや、IKがターゲットポーズを変更したい場合レイヤーを介して行う。
	データは作成時に、SkeletonDataをもらうことで作成できるようにする。
	つまり、各ターゲットポーズを触りたいシステムなどは、レイヤーを取得して触れるようにすること。
	cppいらない説があるけど一旦残しとく
	優先度別にソートする部分をどっかに置いておかないとダメ。
*/
class PoseLayerStorage :public DataStorageBase<PoseLayerID>
{
public:
	// ポーズレイヤー本体
	MD_STORAGE_ID_READ_WRITE_COLUMN(PoseLayerID, PoseLayer, PoseLayer, poseLayers)
	// 自身のID
	MD_STORAGE_ID_READ_ONLY_COLUMN(PoseLayerID, PoseLayerID, ID, ids)
	// Ownerは生成後に変えない
	MD_STORAGE_ID_READ_ONLY_COLUMN(PoseLayerID, EntityID, OwnerEntity, ownerEntities)
public:
	// 作成
	PoseLayerID Create(EntityID _entity, SkeletonID _skeletonID, const PoseBuffer& _targetPose);
	// 破棄
	void Destroy(PoseLayerID _id);
};