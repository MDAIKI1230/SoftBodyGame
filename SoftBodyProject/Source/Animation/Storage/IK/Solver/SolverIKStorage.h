#pragma once

#include <memory>

#include "StorageAccessorsMacros.h"

#include "DataStorageBase.h"

#include "SolverIKID.h"

#include "TwoBoneIKStorage.h"

class SolverIKStorage :public DataStorageBase<SolverIKID>
{
public:
	// TwoBoneIK
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(SolverIKID, TwoBoneIK, TwoBoneIK, twoBoneIKStorage, IK)
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(SolverIKID, PoseLayerID, TwoBoneIKPoseLayerID, twoBoneIKStorage, PoseLayerID)
	MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(SolverIKID, SkeletonID, TwoBoneIKSkeletonID, twoBoneIKStorage, SkeletonID)
	MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(SolverIKID, SolverIKID, TwoBoneIKID, twoBoneIKStorage, ID)
public:
	// コンストラクタ
	SolverIKStorage();
	// 2関節IK作成
	SolverIKID CreateTwoBoneIK(PoseLayerID _poseLayerID, SkeletonID _skeletonID);
	// 破棄
	void Destroy(SolverIKID _id);
private:
	std::unique_ptr<TwoBoneIKStorage> twoBoneIKStorage;
};