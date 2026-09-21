#pragma once

#include "StorageAccessorsMacros.h"

#include "FeatureIKID.h"
#include "SolverIKID.h"
#include "SkeletonID.h"

#include "HandIK.h"

class HandIKStorage
{
public:
	// 実データ
	MD_STORAGE_READ_WRITE_COLUMN(HandIK, IK, iks)
	// 解消用IKデータID
	MD_STORAGE_READ_ONLY_COLUMN(SolverIKID, SolverIKID, solverIDs)
	// スケルトンID
	MD_STORAGE_READ_ONLY_COLUMN(SkeletonID, SkeletonID, skeletonIDs)
	// IDs
	MD_STORAGE_READ_ONLY_COLUMN(FeatureIKID, ID, ids)
public:
	// 追加
	void Add(FeatureIKID _id, SolverIKID _solverIDs, SkeletonID _skeletonID, HandIK _ik);
	// 破棄
	FeatureIKID Remove(uint32_t _index);
};