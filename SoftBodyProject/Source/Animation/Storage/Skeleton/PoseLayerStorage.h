#pragma once

#include "StorageAccessorsMacros.h"

#include "DataStorageBase.h"

#include "PoseLayerID.h"
#include "SkeletonID.h"

#include "PoseLayer.h"
#include "SkeletonInstanceData.h"

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
public:
	// 作成
	PoseLayerID Create(SkeletonID _skeletonID, const SkeletonInstanceData& _skeleton, const char* _maskPath);
	// 破棄
	void Destroy(PoseLayerID _id);

public:
	// マスク生成
	bool CreateMask(const char* _path, const SkeletonData* _skeleton, BoneMask& _output);
};