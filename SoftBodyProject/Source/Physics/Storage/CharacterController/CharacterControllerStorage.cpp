#include "CharacterControllerStorage.h"

// 作成
CharacterControllerID CharacterControllerStorage::Create(EntityID _entity, PhysicsTransformID _transformID, BodyID _rigidBodyID, ColliderID _capsuleColliderID)
{
	if (Has(_entity))
	{
		return {};
	}

	uint32_t denseIndex{ static_cast<uint32_t>(ids.size()) };

	CharacterControllerID id{ CreateID(denseIndex) };

	// --- 操作要求 ---

	moveInputs.emplace_back(Vector3::ZERO);
	jumpRequests.emplace_back(false);
	motorEnableds.emplace_back(true);

	// --- 移動設定 ---

	maxSpeeds.emplace_back(500.0f);
	groundAccelerations.emplace_back(2000.0f);
	groundDecelerations.emplace_back(2500.0f);
	airAccelerations.emplace_back(500.0f);
	jumpSpeeds.emplace_back(600.0f);
	slopeAccelerations.emplace_back(500.0f);

	// --- 接地設定 ---

	groundProbeDistances.emplace_back(10.0f);
	minGroundDots.emplace_back(0.70710678f);

	// --- 実行時状態 ---

	states.emplace_back(CharacterGroundState::WALKABLE);
	groundNormals.emplace_back(Vector3::UP);
	groundPoints.emplace_back(Vector3::ZERO);
	groundDistances.emplace_back(0.0f);
	groundColliderIDs.emplace_back(ColliderID{});

	// --- ID関連 ---

	ids.emplace_back(id);
	transformIDs.emplace_back(_transformID);
	rigidBodyIDs.emplace_back(_rigidBodyID);
	capsuleColliderIDs.emplace_back(_capsuleColliderID);
	ownerEntities.push_back(_entity);

	// MAPに登録
	entityMap.emplace(_entity, id);

	return id;
}

// 破棄
void CharacterControllerStorage::Destroy(CharacterControllerID _id)
{
	if (!IsAlive(_id))
	{
		return;
	}

	uint32_t denseIndex{ GetDenseIndex(_id) };
	uint32_t lastIndex{ static_cast<uint32_t>(ids.size() - 1) };
	CharacterControllerID movedID{ ids[lastIndex] };

	if (denseIndex != lastIndex)
	{
		// --- 操作要求 ---

		moveInputs[denseIndex] = moveInputs[lastIndex];
		jumpRequests[denseIndex] = jumpRequests[lastIndex];
		motorEnableds[denseIndex] = motorEnableds[lastIndex];

		// --- 移動設定 ---

		maxSpeeds[denseIndex] = maxSpeeds[lastIndex];
		groundAccelerations[denseIndex] = groundAccelerations[lastIndex];
		groundDecelerations[denseIndex] = groundDecelerations[lastIndex];
		airAccelerations[denseIndex] = airAccelerations[lastIndex];
		jumpSpeeds[denseIndex] = jumpSpeeds[lastIndex];
		slopeAccelerations[denseIndex] = slopeAccelerations[lastIndex];

		// --- 接地設定 ---

		groundProbeDistances[denseIndex] = groundProbeDistances[lastIndex];
		minGroundDots[denseIndex] = minGroundDots[lastIndex];

		// --- 実行時状態 ---

		states[denseIndex] = states[lastIndex];
		groundNormals[denseIndex] = groundNormals[lastIndex];
		groundPoints[denseIndex] = groundPoints[lastIndex];
		groundDistances[denseIndex] = groundDistances[lastIndex];
		groundColliderIDs[denseIndex] = groundColliderIDs[lastIndex];

		// --- ID関連 ---

		ids[denseIndex] = ids[lastIndex];
		transformIDs[denseIndex] = transformIDs[lastIndex];
		rigidBodyIDs[denseIndex] = rigidBodyIDs[lastIndex];
		capsuleColliderIDs[denseIndex] = capsuleColliderIDs[lastIndex];
		ownerEntities[denseIndex] = ownerEntities[lastIndex];
	}

	// --- 操作要求 ---

	moveInputs.pop_back();
	jumpRequests.pop_back();
	motorEnableds.pop_back();

	// --- 移動設定 ---

	maxSpeeds.pop_back();
	groundAccelerations.pop_back();
	groundDecelerations.pop_back();
	airAccelerations.pop_back();
	jumpSpeeds.pop_back();
	slopeAccelerations.pop_back();

	// --- 接地設定 ---

	groundProbeDistances.pop_back();
	minGroundDots.pop_back();

	// --- 実行時状態 ---

	states.pop_back();
	groundNormals.pop_back();
	groundPoints.pop_back();
	groundDistances.pop_back();
	groundColliderIDs.pop_back();

	// --- ID関連 ---

	ids.pop_back();
	transformIDs.pop_back();
	rigidBodyIDs.pop_back();
	capsuleColliderIDs.pop_back();

	if (denseIndex != lastIndex)
	{
		EditDenseIndex(movedID) = denseIndex;
	}

	entityMap.erase(GetOwnerEntity(_id));

	ReleaseID(_id);
}

// EntityがCharacterControllerを持っているか
bool CharacterControllerStorage::Has(EntityID _entity) const
{
	return entityMap.contains(_entity);
}

// Entityに対応するCharacterControllerIDを取得
bool CharacterControllerStorage::TryGet(EntityID _entity, CharacterControllerID& _output) const
{
	const auto it{ entityMap.find(_entity) };

	if (it == entityMap.end())
	{
		return false;
	}

	_output = it->second;

	return true;
}
