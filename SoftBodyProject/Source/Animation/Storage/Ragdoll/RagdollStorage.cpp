#include "PhysicsComponentAPI.h"

#include "RagdollStorage.h"

// 生成
RagdollID RagdollStorage::Create(EntityID _entity, SkeletonID _skeletonID, ModelHandle _model)
{
	RagdollID result{ CreateID(CountID()) };

	Ragdoll ragdoll;

	ragdoll.skeleton = _skeletonID;

    // 全ボーンでforeach
        // 物理トランスフォーム作る
        // リンク作成
        // 拘束作成
        // 拘束うまく繋げる

    ragdolls.push_back(std::move(ragdoll));

	neesInitializes.emplace_back(true);

	ids.push_back(result);

	ownerEntities.push_back(_entity);

	return result;
}

// 破棄
void RagdollStorage::Destroy(RagdollID _id)
{
    // 生存確認
    if (!IsAlive(_id))
    {
        return;
    }

    uint32_t denseIndex{ GetDenseIndex(_id) };
    uint32_t lastIndex{ static_cast<uint32_t>(ids.size() - 1) };

    for (auto transformID : ragdolls[denseIndex].transforms)
    {
        PhysicsComponentAPI::DestroyPhysicsTransform(transformID);
    }

    // Swap＆Pop
    if (denseIndex != lastIndex)
    {
        RagdollID movedID{ ids[lastIndex] };

        ragdolls[denseIndex] = std::move(ragdolls[lastIndex]);

		neesInitializes[denseIndex] = neesInitializes[lastIndex];

        ownerEntities[denseIndex] = ownerEntities[lastIndex];

        ids[denseIndex] = movedID;

        // 移動したInstanceの参照先を更新
        EditDenseIndex(movedID) = denseIndex;
    }

    ragdolls.pop_back();
	neesInitializes.pop_back();
    ownerEntities.pop_back();
    ids.pop_back();

    ReleaseID(_id);
}
