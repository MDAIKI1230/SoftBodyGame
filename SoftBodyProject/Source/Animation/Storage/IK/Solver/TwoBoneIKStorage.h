#pragma once

#include "StorageAccessorsMacros.h"

#include "SolverIKID.h"
#include "PoseLayerID.h"
#include "SkeletonID.h"

#include "TwoBoneIK.h"

class TwoBoneIKStorage
{
public:
	// 実データ
	MD_STORAGE_READ_WRITE_COLUMN(TwoBoneIK, IK, iks)
	// 書き込みレイヤーID
	MD_STORAGE_READ_WRITE_COLUMN(PoseLayerID, PoseLayerID, poseLayerIDs)
	// スケルトンID
	MD_STORAGE_READ_ONLY_COLUMN(SkeletonID, SkeletonID, skeletonIDs)
	// IDs
	MD_STORAGE_READ_ONLY_COLUMN(SolverIKID, ID, ids)
public:
	// 追加
	void Add(SolverIKID _id, PoseLayerID _poseLayerID, SkeletonID _skeletonID, TwoBoneIK _ik);
	// 破棄
	SolverIKID Remove(uint32_t _index);
};