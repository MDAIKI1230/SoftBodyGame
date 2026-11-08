#include "HandIKStorage.h"

// 追加
void HandIKStorage::Add(FeatureIKID _id, SolverIKID _solverIDs, SkeletonID _skeletonID, HandIK _ik)
{
	iks.push_back(_ik);
	solverIDs.push_back(_solverIDs);
	skeletonIDs.push_back(_skeletonID);
	ids.push_back(_id);
}

// 破棄
FeatureIKID HandIKStorage::Remove(uint32_t _index)
{
	if (ids.empty())
	{
		return {};
	}

	size_t last{ ids.size() - 1 };
	FeatureIKID movedId{ ids[last]};

	if (_index != last)
	{
		iks[_index] = iks[last];
		solverIDs[_index] = solverIDs[last];
		skeletonIDs[_index] = skeletonIDs[last];
		ids[_index] = ids[last];
	}

	iks.pop_back();
	solverIDs.pop_back();
	skeletonIDs.pop_back();
	ids.pop_back();

	return movedId;
}