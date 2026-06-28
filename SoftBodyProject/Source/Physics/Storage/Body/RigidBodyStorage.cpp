#include "RigidBodyStorage.h"


BodyID RigidBodyStorage::CreateRigidBody(EntityID _entity, PhysicsTransformID _transformID)
{
	// --- 値を確保 ---

	// --- 速度系 ---

	// 力
	force.emplace_back();
	// 速度
	velocity.emplace_back();

	// --- 角速度系 ---

	// トルク
	torque.emplace_back();
	// 角速度
	angularVelocity.emplace_back();

	// --- 重力系 ---

	// 重力フラグ
	isGravity.emplace_back(true);
	// 重力加速度
	gravity.emplace_back(0.0, -980.0f, 0.0f);

	// --- 質量系 ---

	// 質量(0除算を避けるため)
	mass.emplace_back(1.0f);
	// 質量の逆数
	inverseMass.emplace_back(1.0f);
	// 慣性テンソルの逆数
	inverseInertiaTensor.emplace_back();

	// マテリアルID(一旦なし)
	physicsMatrialID.emplace_back(-1);

	// TransformID
	transformID.push_back(_transformID);

	// ID
	id.emplace_back(GenerateBodyID(id.size(), _entity));

	return id.back();
}

void RigidBodyStorage::Destroy(BodyID _id)
{
	// 削除Index
	size_t index{ slots[_id.index].denseIndex };

	// 力
	force[index] = std::move(force.back());
	force.pop_back();
	// 速度
	velocity[index] = std::move(velocity.back());
	velocity.pop_back();

	// --- 角速度系 ---

	// トルク
	torque[index] = std::move(torque.back());
	torque.pop_back();
	// 角速度
	angularVelocity[index] = std::move(angularVelocity.back());
	angularVelocity.pop_back();

	// --- 重力系 ---

	// 重力フラグ
	isGravity[index] = std::move(isGravity.back());
	isGravity.pop_back();
	// 重力加速度
	gravity[index] = std::move(gravity.back());
	gravity.pop_back();

	// --- 質量系 ---

	// 質量(0除算を避けるため)
	// 推定回転角度
	mass[index] = std::move(mass.back());
	mass.pop_back();
	// 質量の逆数
	inverseMass[index] = std::move(inverseMass.back());
	inverseMass.pop_back();
	// 慣性テンソルの逆数
	inverseInertiaTensor[index] = std::move(inverseInertiaTensor.back());
	inverseInertiaTensor.pop_back();

	// マテリアルID(一旦なし)
	physicsMatrialID[index] = std::move(physicsMatrialID.back());
	physicsMatrialID.pop_back();

	// TransformID
	transformID[index] = std::move(transformID.back());
	transformID.pop_back();

	// swap-removeしたときの移動したID
	BodyID movedId{ id[index] };

	// 移動後のIDの修正
	slots[movedId.index].denseIndex = slots[_id.index].denseIndex;

	// フリーに追加
	freeSlots.push_back(_id.index);
	slots[_id.index].generation++;
}

bool RigidBodyStorage::IsAlive(BodyID _id) const
{
	return slots[_id.index].alive && slots[_id.index].generation == _id.generation;
}

uint32_t RigidBodyStorage::GetDenseIndex(BodyID _id) const
{
	return slots[_id.index].denseIndex;
}

EntityID RigidBodyStorage::GetOwnerEntity(BodyID _id) const
{
	return slots[_id.index].ownerEntity;
}

BodyID RigidBodyStorage::GenerateBodyID(size_t _denseIndex, EntityID _ownerEntity)
{
	if (freeSlots.empty())
	{
		// --- フリーのスロットがないため新たにスロットを作成---

		// IDを作成(初代判定で1)
		BodyID result{ slots.size(),1 };
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

		// IDを作成(初代判定で1)
		return BodyID{ (uint32_t)(index),slots[index].generation };
	}
}
