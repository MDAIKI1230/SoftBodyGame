#include "AnimationStorage.h"

// 作成関数
AnimationID AnimationStorage::Create(EntityID _entity, ModelHandle _model, PoseLayerID _layerID)
{
	AnimationID id{ CreateID(CountID()) };

	AnimationInstanceData data;

	data.modelHandle = _model;
	data.layerID = _layerID;

	animationInstanceDatas.push_back(data);
	ids.push_back(id);
	ownerEntities.push_back(_entity);

	return id;
}

// 破棄
void AnimationStorage::Destroy(AnimationID _id)
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
		AnimationID movedID{ ids[lastIndex] };

		animationInstanceDatas[denseIndex] = std::move(animationInstanceDatas[lastIndex]);

		ids[denseIndex] = movedID;

		ownerEntities[denseIndex] = ownerEntities[lastIndex];

		// 移動したInstanceの参照先を更新
		EditDenseIndex(movedID) = denseIndex;
	}

	animationInstanceDatas.pop_back();
	ownerEntities.pop_back();
	ids.pop_back();

	ReleaseID(_id);
}