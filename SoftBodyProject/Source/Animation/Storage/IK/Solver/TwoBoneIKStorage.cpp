#include "TwoBoneIKStorage.h"

// 追加
void TwoBoneIKStorage::Add(SolverIKID _id, PoseLayerID _poseLayerID, SkeletonID _skeletonID, TwoBoneIK _ik)
{
	iks.push_back(_ik);
	poseLayerIDs.push_back(_poseLayerID);
	skeletonIDs.push_back(_skeletonID);
	ids.push_back(_id);
}
// 破棄
SolverIKID TwoBoneIKStorage::Remove(uint32_t _index)
{
	if (ids.empty())
	{
		return {};
	}

	size_t last{ ids.size() - 1 };
	SolverIKID movedId{ ids[last] };

	if (_index != last)
	{
		iks[_index] = iks[last];
		poseLayerIDs[_index] = poseLayerIDs[last];
		skeletonIDs[_index] = skeletonIDs[last];
		ids[_index] = ids[last];
	}

	iks.pop_back();
	poseLayerIDs.pop_back();
	skeletonIDs.pop_back();
	ids.pop_back();

	return movedId;
}