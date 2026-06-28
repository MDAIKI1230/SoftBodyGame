#include "PhysicsTransformStorage.h"

PhysicsTransformID PhysicsTransformStorage::CreateTransform(EntityID _entity)
{
	// --- 実際のデータを追加 ---

	// 位置
	position.emplace_back();
	// 回転
	rotation.emplace_back();
	// スケール
	scale.emplace_back();
	// ローカル行列
	localMatrix.emplace_back();
	// ワールド行列
	worldMatrix.emplace_back();
	// 親ID
	parentID.emplace_back();
	// ID
	id.emplace_back(GeneratePhysicsTransformID(id.size(), _entity));

	// IDを返してあげる
	return id.back();
}

void PhysicsTransformStorage::Destroy(PhysicsTransformID _id)
{

}

bool PhysicsTransformStorage::IsAlive(PhysicsTransformID _id) const
{
	return slots[_id.index].alive && slots[_id.index].generation == _id.generation;
}

size_t PhysicsTransformStorage::GetDenseIndex(PhysicsTransformID _id) const
{
	return slots[_id.index].denseIndex;
}

EntityID PhysicsTransformStorage::GetOwnerEntity(PhysicsTransformID _id) const
{
	return slots[_id.index].ownerEntity;
}

PhysicsTransformID PhysicsTransformStorage::GeneratePhysicsTransformID(size_t _denseIndex, EntityID _ownerEntity)
{
	if (freeSlots.empty())
	{
		// --- フリーのスロットがないため新たにスロットを作成---

		// IDを作成(初代判定で1)
		PhysicsTransformID result{ slots.size(),1 };
		// Slotを増設
		slots.emplace_back(_denseIndex, _ownerEntity);

		return result;
	}
	else
	{
		// フリーのスロットがあるためそれを使用

		// 最後を取る
		size_t index{ freeSlots.back() };
		freeSlots.pop_back();

		// 世代は削除時に加算済み
		slots[index].alive = true;
		slots[index].denseIndex = _denseIndex;
		slots[index].ownerEntity = _ownerEntity;

		// IDを作成
		return PhysicsTransformID{ (uint32_t)(index),slots[index].generation };
	}
}
