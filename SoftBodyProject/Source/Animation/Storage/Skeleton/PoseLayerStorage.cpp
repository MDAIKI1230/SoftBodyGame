#include "PoseLayerStorage.h"

// 作成
PoseLayerID PoseLayerStorage::Create(SkeletonID _skeletonID, const PoseBuffer& _targetPose)
{
	PoseLayerID id{ CreateID(CountID()) };

	PoseLayer poseLayer;

	poseLayer.skeletonID = _skeletonID;
	poseLayer.pose = _targetPose;
	poseLayer.mask.weights.resize(_targetPose.Size());

	poseLayers.push_back(std::move(poseLayer));
	ids.push_back(id);
	
	return id;
}

// 破棄
void PoseLayerStorage::Destroy(PoseLayerID _id)
{
	// 生存確認
	if (!IsAlive(_id))
	{
		return;
	}

	uint32_t denseIndex{ GetDenseIndex(_id) };
	uint32_t lastIndex{ static_cast<uint32_t>(ids.size() - 1) };

	// Swap＆Pop
	if (denseIndex != lastIndex)
	{
		PoseLayerID movedID{ ids[lastIndex] };

		poseLayers[denseIndex] = std::move(poseLayers[lastIndex]);

		ids[denseIndex] = movedID;

		// 移動したInstanceの参照先を更新
		EditDenseIndex(movedID) = denseIndex;
	}

	poseLayers.pop_back();
	ids.pop_back();

	ReleaseID(_id);
}