#include "BodyStorage.h"

// 質量の代入
void BodyStorage::SetRigidBodyMass(BodyID _id, float _mass)
{
	if (!IsAlive(_id))
	{
		return;
	}

	rigidBodyStorage->SetMass(GetDenseIndex(_id), _mass);
}

// 計算が完了したときに呼ぶ関数
void BodyStorage::LocalInertiaCalcSucces(BodyID _id)
{
	if (!IsAlive(_id))
	{
		return;
	}

	rigidBodyStorage->LocalInertiaCalcSucces(GetDenseIndex(_id));
}

// 回転制限マスク変更関数
void BodyStorage::SetRigidBodyRotationLock(BodyID _id, RigidBodyRotationLock _lock)
{
	if (!IsAlive(_id))
	{
		return;
	}

	rigidBodyStorage->SetRotationLock(GetDenseIndex(_id), _lock);
}

// 移動制限マスク変更関数
void BodyStorage::SetRigidBodyPositionLock(BodyID _id, RigidBodyPositionLock _lock)
{
	if (!IsAlive(_id))
	{
		return;
	}

	rigidBodyStorage->SetPositionLock(GetDenseIndex(_id), _lock);
}

// コンストラクタ
BodyStorage::BodyStorage()
{
	rigidBodyStorage = std::make_unique<RigidBodyStorage>();
	ropeStorage = std::make_unique<RopeStorage>();
	clothStorage = std::make_unique<ClothStorage>();
	softBodyStorage = std::make_unique<SoftBodyStorage>();
}

// RigidBody作成
BodyID BodyStorage::CreateRigidBody(EntityID _entity, PhysicsTransformID _transformID)
{
	BodyID result{ CreateID(rigidBodyStorage->CountID(),BodyType::RIGID_BODY,_entity,_transformID) };

	rigidBodyStorage->Create(result);

	// Map対応付け
	transformMap[_transformID] = result;

	return result;
}
// Rope作成
BodyID BodyStorage::CreateRope(EntityID _entity, PhysicsTransformID _transformID, const RopeUpdateInfo& _info)
{
	BodyID result{ CreateID(ropeStorage->CountID(),BodyType::ROPE,_entity,_transformID)};

	ropeStorage->CreateRope(result, _info);

	return result;
}
// Cloth作成
BodyID BodyStorage::CreateCloth(EntityID _entity, PhysicsTransformID _transformID, const ClothUpdateInfo& _info)
{
	BodyID result{ CreateID(clothStorage->CountID(),BodyType::CLOTH,_entity,_transformID)};

	clothStorage->Create(result, _info);

	return result;
}
// SoftBody作成
BodyID BodyStorage::CreateSoftBody(EntityID _entity, PhysicsTransformID _transformID, const SoftBodyUpdateInfo& _info)
{
	BodyID result{ CreateID(softBodyStorage->CountID(),BodyType::SOFT_BODY,_entity,_transformID) };

	softBodyStorage->Create(result, _info);
	return result;
}

// 回転制限マスク適応済み逆慣性テンソル適応角速度関数
Vector3 BodyStorage::ApplyAngularInverseInertia(const Matrix4x4& _worldInverseInertia, const Vector3& _angularImpulse, const Vector3& _rotationLockMasks)
{
	const Vector3 maskedImpulse{ SIMDVectorMath::Mul(_angularImpulse,_rotationLockMasks) };

	const Vector3 response{ _worldInverseInertia * maskedImpulse };

	return SIMDVectorMath::Mul(response, _rotationLockMasks);
}

// 移動制限マスク適応済み逆質量適応速度計算関数
Vector3 BodyStorage::ApplyLinearInverseMass(const Vector3& _impulse, float _inverseMass, const Vector3& _linearFactor)
{
	return SIMDVectorMath::Mul(_impulse * _inverseMass, _linearFactor);
}

// 破棄
void BodyStorage::Destroy(BodyID _id)
{
	if (!IsAlive(_id))
	{
		return;
	}

	// swap-removeしたときの移動したID
	BodyID movedId;

	switch (GetType(_id))
	{
	case BodyType::RIGID_BODY:
		movedId = rigidBodyStorage->Remove(GetDenseIndex(_id));
		break;
	case BodyType::ROPE:
		movedId = ropeStorage->Remove(GetDenseIndex(_id));
		break;
	case BodyType::CLOTH:
		movedId = clothStorage->Remove(GetDenseIndex(_id));
		break;
	case BodyType::SOFT_BODY:
		movedId = softBodyStorage->Remove(GetDenseIndex(_id));
		break;
	default:
		break;
	}

	// Mapから削除
	auto it = transformMap.find(GetTransformID(_id));
	if (it != transformMap.end())
	{
		transformMap.erase(it);
	}

	// 移動後のIDの修正
	EditDenseIndex(movedId) = GetDenseIndex(_id);

	// フリーに追加
	ReleaseID(_id);
}

// 種類取得
BodyType BodyStorage::GetType(BodyID _id) const
{
	return GetSlot(_id).type;
}

// 対応Transform
PhysicsTransformID BodyStorage::GetTransformID(BodyID _id) const
{
	return GetSlot(_id).transformID;
}
// RigidBodyのBodyID取得
bool BodyStorage::TryGetRigidBodyID(PhysicsTransformID _transformID, BodyID& _output)
{
	if (transformMap.contains(_transformID))
	{
		_output = transformMap[_transformID];
		return true;
	}

	return false;
}

// TransformIDと紐づくBodyIDがあるか否か
bool BodyStorage::Has(PhysicsTransformID _transformID) const
{
	return transformMap.contains(_transformID);
}
