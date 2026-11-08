#include "BoneMaskLoader.h"

#include "PoseLayerStorage.h"

// 作成
PoseLayerID PoseLayerStorage::Create(SkeletonID _skeletonID, const SkeletonData* _skeleton, const PoseBuffer& _targetPose, const char* _maskPath)
{
	PoseLayerID id{ CreateID(CountID()) };

	PoseLayer poseLayer;

	poseLayer.skeletonID = _skeletonID;
	poseLayer.pose = _targetPose;
	if (!CreateMask(_maskPath, _skeleton, poseLayer.mask))
	{
		ReleaseID(id);
		return {};
	}

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

// マスク生成
bool PoseLayerStorage::CreateMask(const char* _path, const SkeletonData* _skeleton, BoneMask& _output)
{
	_output.ReSize(_skeleton->Size());

	if (_path == nullptr)
	{
		for (float& weight : _output.weights)
		{
			weight = 1.0f;
		}

		return true;
	}

	return BoneMaskLoader::Load(_path, _skeleton, _output);
}