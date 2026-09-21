#include "SolverIKStorage.h"

// コンストラクタ
SolverIKStorage::SolverIKStorage()
{
	twoBoneIKStorage = std::make_unique<TwoBoneIKStorage>();
}
// 2関節IK作成
SolverIKID SolverIKStorage::CreateTwoBoneIK(PoseLayerID _poseLayerID, SkeletonID _skeletonID)
{
	SolverIKID id{ CreateID(twoBoneIKStorage->CountID()) };

	twoBoneIKStorage->Add(id, _poseLayerID, _skeletonID, {});

	return id;
}
// 破棄
void SolverIKStorage::Destroy(SolverIKID _id)
{
	if (!IsAlive(_id))
	{
		return;
	}

	// 移動インデックス
	SolverIKID movedId{ twoBoneIKStorage->Remove(GetDenseIndex(_id)) };

	// 今後増えたらTYPE追加していこう

	if (!(movedId.GetIndex() == _id.GetIndex() && movedId.GetGeneration() == _id.GetGeneration()))
	{
		EditDenseIndex(movedId) = GetDenseIndex(_id);
	}

	// 削除
	ReleaseID(_id);
}