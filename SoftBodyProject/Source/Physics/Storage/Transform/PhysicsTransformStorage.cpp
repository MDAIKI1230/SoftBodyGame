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
	// ECSとの関係
	syncPolicies.push_back(PhysicsTransformSyncPolicy::BIDIRECTIONAL);
	// 使用エンティティ
	ownerEntities.push_back(_entity);
	// ID
	ids.emplace_back(CreateID(static_cast<uint32_t>(ids.size())));
	// mapに追加
	entityMap[_entity] = ids.back();
	// IDを返してあげる
	return ids.back();
}

// 内部用のTransform作成
PhysicsTransformID PhysicsTransformStorage::CreateInternalTransform(EntityID _entity, const Vector3& _position, const Quaternion& _rotation, const Vector3& _scale)
{
	// --- 実際のデータを追加 ---

	// 位置
	positions.push_back(_position);
	// 回転
	rotations.push_back(_rotation);
	// スケール
	scales.push_back(_scale);
	// ローカル行列
	localMatrices.emplace_back();
	// ワールド行列
	worldMatrices.emplace_back();
	// 親ID
	parentIDs.emplace_back();
	// ECSとの関係
	syncPolicies.push_back(PhysicsTransformSyncPolicy::NONE);
	// 使用エンティティ
	ownerEntities.push_back(_entity);
	// ID
	ids.emplace_back(CreateID(static_cast<uint32_t>(ids.size())));
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
		// ECSとの関係
		syncPolicies[index] = syncPolicies[last];
		// 使用エンティティ
		ownerEntities[index] = ownerEntities[last];
		// ID
		ids[index] = ids[last];
	}

	positions.pop_back();
	rotations.pop_back();
	scales.pop_back();
	localMatrices.pop_back();
	worldMatrices.pop_back();
	parentIDs.pop_back();
	syncPolicies.pop_back();
	ownerEntities.pop_back();
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
