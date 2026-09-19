#include "ResourceManager.h"

#include "SkeletonInstanceStorage.h"

SkeletonID SkeletonInstanceStorage::Create(EntityID _entity, ModelHandle _model)
{
	// ID作成
	SkeletonID id{ CreateID(CountID()) };

	// データ作っていく
	SkeletonInstanceData skeletonInstanceData;
	skeletonInstanceData.skeletonData = ResourceManager::GetSkeletonData(_model);
	ResourceManager::GetPose(_model, skeletonInstanceData.outputPose);
	skeletonInstanceData.targetPose = skeletonInstanceData.outputPose;

	skeletonInstanceDatas.push_back(std::move(skeletonInstanceData));

	worldFromModels.emplace_back();
	ids.push_back(id);
	ownerEntities.push_back(_entity);

	entityLookUp[_entity] = id;

	return id;
}

void SkeletonInstanceStorage::Destroy(SkeletonID _id)
{
    // 生存確認
    if (!IsAlive(_id))
    {
        return;
    }

    uint32_t denseIndex{ GetDenseIndex(_id) };
    uint32_t lastIndex{ static_cast<uint32_t>(ids.size() - 1) };

	entityLookUp.erase(ownerEntities[denseIndex]);

    // Swap＆Pop
    if (denseIndex != lastIndex)
    {
        SkeletonID movedID{ ids[lastIndex] };

        skeletonInstanceDatas[denseIndex] = std::move(skeletonInstanceDatas[lastIndex]);

		worldFromModels[denseIndex] = worldFromModels[lastIndex];

        ownerEntities[denseIndex] = ownerEntities[lastIndex];

        ids[denseIndex] = movedID;

        // 移動したInstanceの参照先を更新
        EditDenseIndex(movedID) = denseIndex;
    }

    skeletonInstanceDatas.pop_back();
	worldFromModels.pop_back();
    ownerEntities.pop_back();
    ids.pop_back();

    ReleaseID(_id);
}

// エンティティと対応したSkeletonIDがあるならそれを返すないなら作って返す
SkeletonID SkeletonInstanceStorage::CreateOrGetID(EntityID _entity, ModelHandle _model)
{
	if (entityLookUp.contains(_entity))
	{
		return entityLookUp[_entity];
	}

	return Create(_entity, _model);
}
