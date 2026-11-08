#pragma once

#include <memory>

#include "StorageAccessorsMacros.h"

#include "DataStorageBase.h"

#include "FeatureIKID.h"

#include "HandIKStorage.h"

class FeatureIKStorage :public DataStorageBase<FeatureIKID>
{
public:
	// HandIK
	MD_OWNED_STORAGE_READ_WRITE_ACCESSORS(FeatureIKID, HandIK, HandIK, handIKStorage, IK)
	MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(FeatureIKID, SolverIKID, HandSolverIKID, handIKStorage, SolverIKID)
	MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(FeatureIKID, SkeletonID, HandSkeletonID, handIKStorage, SkeletonID)
	MD_OWNED_STORAGE_READ_ONLY_ACCESSORS(FeatureIKID, FeatureIKID, HandIKID, handIKStorage, ID)
public:
	// コンストラクタ
	FeatureIKStorage();
	// ハンドIK作成
	FeatureIKID CreateHandIK(SolverIKID _solverIKID, SkeletonID _skeletonID);
	// 破棄
	void Destroy(FeatureIKID _id);
private:
	std::unique_ptr<HandIKStorage> handIKStorage;
};