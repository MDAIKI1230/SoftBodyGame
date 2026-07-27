#include "PhysicsTransformStorage.h"

PhysicsTransformID PhysicsTransformStorage::GetOrCreateTransform(EntityID _entity)
{
	if (entityMap.contains(_entity))
	{
		return entityMap[_entity];
	}

	// --- 実際のデータを追加 ---

	// 位置
	positions.emplace_back();
	// 回転
	rotations.emplace_back();
	// スケール
	scales.emplace_back();
	// ローカル行列
	localMatrices.emplace_back();
	// ワールド行列
	worldMatrices.emplace_back();
	// 親ID
	parentIDs.emplace_back();
	// ID
	ids.emplace_back(CreateID(static_cast<uint32_t>(ids.size()), _entity));
	// mapに追加
	entityMap[_entity] = ids.back();
	// IDを返してあげる
	return ids.back();
}

void PhysicsTransformStorage::Destroy(PhysicsTransformID _id)
{
	uint32_t index{ GetDenseIndex(_id) };

	if (ids.empty())
	{
		return;
	}

	size_t last = ids.size() - 1;
	PhysicsTransformID movedId = ids[last];

	if (index != last)
	{
		// 位置
		positions[index] = positions[last];
		// 回転
		rotations[index] = rotations[last];
		// スケール
		scales[index] = scales[last];
		// ローカル行列
		localMatrices[index] = localMatrices[last];
		// ワールド行列
		worldMatrices[index] = worldMatrices[last];
		// 親ID
		parentIDs[index] = parentIDs[last];
		// ID
		ids[index] = ids[last];
	}

	positions.pop_back();
	rotations.pop_back();
	scales.pop_back();
	localMatrices.pop_back();
	worldMatrices.pop_back();
	ids.pop_back();

	auto it = entityMap.find(GetOwnerEntity(_id));

	if (!entityMap.empty())
	{
		entityMap.erase(it);
	}

	if (!(movedId.GetIndex() == _id.GetIndex() && movedId.GetGeneration() == _id.GetGeneration()))
	{
		EditDenseIndex(movedId) = GetDenseIndex(_id);
	}

	ReleaseID(_id);
}

bool PhysicsTransformStorage::TryGet(EntityID _entity, PhysicsTransformID& _output)
{
	// あったらアウトプットに入れてないならflaseで終了
	if (entityMap.contains(_entity))
	{
		_output = entityMap[_entity];
		return true;
	}

	return false;
}
