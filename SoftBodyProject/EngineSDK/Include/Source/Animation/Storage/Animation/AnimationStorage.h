#pragma once

#include "StorageAccessorsMacros.h"

#include "DataStorageBase.h"

#include "AnimationID.h"

#include "AnimationInstanceData.h"

/*
	アニメーションの再生状態を管理する用ストレージ
*/
class AnimationStorage :public DataStorageBase<AnimationID>
{
public:
	// アニメーションの実行時データ
	MD_STORAGE_ID_READ_WRITE_COLUMN(AnimationID, AnimationInstanceData, AnimationInstanceData, animationInstanceDatas);
	// 自身のID
	MD_STORAGE_ID_READ_ONLY_COLUMN(AnimationID, AnimationID, ID, ids)
public:
	// 作成関数
	AnimationID Create(ModelHandle _model, PoseLayerID _layerID);
	// 破棄
	void Destroy(AnimationID _id);
};