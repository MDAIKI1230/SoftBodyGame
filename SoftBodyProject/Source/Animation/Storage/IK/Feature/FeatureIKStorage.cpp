#include "FeatureIKStorage.h"

// コンストラクタ
FeatureIKStorage::FeatureIKStorage()
{
	handIKStorage = std::make_unique<HandIKStorage>();
}
// ハンドIK作成
FeatureIKID FeatureIKStorage::CreateHandIK(SolverIKID _solverIKID, SkeletonID _skeletonID)
{
	FeatureIKID id{ CreateID(handIKStorage->CountID()) };

	handIKStorage->Add(id, _solverIKID, _skeletonID, {});

	return id;
}

// 破棄
void FeatureIKStorage::Destroy(FeatureIKID _id)
{
	if (!IsAlive(_id))
	{
		return;
	}

	// 移動インデックス
	FeatureIKID movedId{ handIKStorage->Remove(GetDenseIndex(_id)) };

	// 今後増えたらTYPE追加していこう

	if (!(movedId.GetIndex() == _id.GetIndex() && movedId.GetGeneration() == _id.GetGeneration()))
	{
		EditDenseIndex(movedId) = GetDenseIndex(_id);
	}

	// 削除
	ReleaseID(_id);
}