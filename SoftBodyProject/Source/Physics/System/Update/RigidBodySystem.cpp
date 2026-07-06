#include "ServiceLocator.h"

#include "RigidBodySystem.h"


void RigidBodySystem::FixedUpdate(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, ColliderStorage* _colliderStorage)
{
	// 各処理実行
	UpdateGravity(_bodyStorage);
	UpdatePosition(_transformStorage, _bodyStorage);
	UpdateInverseInertiaTensor(_transformStorage, _bodyStorage, _colliderStorage);
	UpdateRotation(_transformStorage, _bodyStorage);
	End(_bodyStorage, _colliderStorage);
}

void RigidBodySystem::UpdateGravity(RigidBodyStorage* _bodyStorage)
{
	for (auto& bodyID : _bodyStorage->id)
	{
		// 実データインデックスの取得
		uint32_t bodyIndex{ _bodyStorage->GetDenseIndex(bodyID) };
		// フラグ判定
		if (_bodyStorage->isGravity[bodyIndex])
		{
			// 質量×重力加速度(Δtに離散化)を力に加算
			_bodyStorage->force[bodyIndex] += _bodyStorage->gravity[bodyIndex] * ServiceLocator::GetTimeManager()->GetFixedDeltaTime() * _bodyStorage->mass[bodyIndex];
		}
	}
}

void RigidBodySystem::UpdatePosition(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage)
{
	for (auto& bodyID : _bodyStorage->id)
	{
		// 実データインデックスの取得
		uint32_t bodyIndex{ _bodyStorage->GetDenseIndex(bodyID) };
		PhysicsTransformID transformID{ _bodyStorage->GetTransformID(bodyID) };
		uint32_t transIndex{ _transformStorage->GetDenseIndex(transformID) };
		// 速度 + 加速度(力(Δt) * 質量の逆数)
		_bodyStorage->velocity[bodyIndex] += _bodyStorage->force[bodyIndex] * ServiceLocator::GetTimeManager()->GetFixedDeltaTime() * _bodyStorage->inverseMass[bodyIndex];

		// 位置保存
		_bodyStorage->pastPos[bodyIndex] = _transformStorage->position[transIndex];

		// 今の位置 + 速度
		_transformStorage->position[transIndex] += _bodyStorage->velocity[bodyIndex] * ServiceLocator::GetTimeManager()->GetFixedDeltaTime();
	}
}

void RigidBodySystem::UpdateRotation(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage)
{
	for (auto& bodyID : _bodyStorage->id)
	{
		// 実データインデックスの取得
		uint32_t bodyIndex{ _bodyStorage->GetDenseIndex(bodyID) };
		PhysicsTransformID transformID{ _bodyStorage->GetTransformID(bodyID) };
		uint32_t transIndex{ _transformStorage->GetDenseIndex(transformID) };
		// 角速度＋ 角加速度(トルク×慣性テンソルの逆数)
		_bodyStorage->angularVelocity[bodyIndex] += _bodyStorage->worldInverseInertiaTensor[bodyIndex] * _bodyStorage->torque[bodyIndex] * ServiceLocator::GetTimeManager()->GetFixedDeltaTime();

		// 角速度と慣性テンソルの逆行列からΔt分の四元数を作成
		// Δω
		Vector3 deltaAngularVelocity{ _bodyStorage->angularVelocity[bodyIndex] * ServiceLocator::GetTimeManager()->GetFixedDeltaTime() };
		// Δωの四元数を作る
		Quaternion rotOmega{ Quaternion::AngleAxis(deltaAngularVelocity.Length(),deltaAngularVelocity) };

		// 姿勢保存
		_bodyStorage->pastRot[bodyIndex] = _transformStorage->rotation[transIndex];

		// 今の回転＋トルク(Δtに離散化)×慣性テンソルの逆行列
		_transformStorage->rotation[transIndex] *= rotOmega;
		// 一応の正規化
		_transformStorage->rotation[transIndex].Normalize();
	}
}

void RigidBodySystem::UpdateInverseInertiaTensor(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage, ColliderStorage* _colliderStorage)
{
	for (auto& bodyID:_bodyStorage->id)
	{
		// 実データインデックスの取得
		uint32_t bodyIndex{ _bodyStorage->GetDenseIndex(bodyID) };
		PhysicsTransformID transformID{ _bodyStorage->GetTransformID(bodyID) };
		uint32_t transIndex{ _transformStorage->GetDenseIndex(transformID) };
		// ローカル慣性テンソルの計算
		if (_bodyStorage->localInertiaDirty[bodyIndex] == true)
		{
			for (auto& colliderID : _colliderStorage->GetColliderIDFromTransformID(transformID))
			{
				
				switch (_colliderStorage->GetType(colliderID))
				{
				case ColliderType::SPHERE:
					_bodyStorage->localInverseInertiaTensor[bodyIndex] = GenerateSphereInverseInertiaTensor(transIndex, colliderID, _bodyStorage->mass[bodyIndex], _transformStorage, _colliderStorage);
					break;
				case ColliderType::BOX:
					_bodyStorage->localInverseInertiaTensor[bodyIndex] = GenerateBoxInverseInertiaTensor(transIndex, colliderID, _bodyStorage->mass[bodyIndex], _transformStorage, _colliderStorage);
					break;
				}
			}
			// フラグを戻す
			_bodyStorage->localInertiaDirty[bodyIndex] = false;
		}
		// 回転行列取得
		Matrix4x4 rotMat{ MatGenerateFunc::Rotate(_transformStorage->rotation[transIndex]) };
		_bodyStorage->worldInverseInertiaTensor[bodyIndex] = rotMat * _bodyStorage->localInverseInertiaTensor[bodyIndex] * rotMat.Transposed();
	}
}

void RigidBodySystem::End(RigidBodyStorage* _bodyStorage, ColliderStorage* _colliderStorage)
{
	for (auto& bodyID : _bodyStorage->id)
	{
		// 実データインデックスの取得
		uint32_t bodyIndex{ _bodyStorage->GetDenseIndex(bodyID) };
		_bodyStorage->force[bodyIndex] = Vector3::ZERO;
		_bodyStorage->torque[bodyIndex] = Vector3::ZERO;

		// AABBのフラグを変更する
		if (_bodyStorage->velocity[bodyIndex].LengthSqr() <= MathConstants::EPSILON &&
			_bodyStorage->angularVelocity[bodyIndex].LengthSqr() <= MathConstants::EPSILON)
		{
			auto& colliders{ _colliderStorage->GetColliderIDFromTransformID(_bodyStorage->GetTransformID(bodyID)) };
			for (auto& colliderID : colliders)
			{
				_colliderStorage->aabbStorage->dirty[_colliderStorage->GetAABBIndex(colliderID)] |= AABBChangeDirtyFlag::TRANSFORM;
			}
		}
	}

	for (auto& aabbDirty : _colliderStorage->aabbStorage->dirty)
	{

		aabbDirty |= AABBChangeDirtyFlag::TRANSFORM;
	}
}


Matrix4x4 RigidBodySystem::GenerateBoxInverseInertiaTensor(const uint32_t& _transformIndex, const ColliderID& _colliderID, float _mass, PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage)
{
	uint32_t index{ _colliderStorage->GetDenseIndex(_colliderID) };
	Vector3 size{ SIMDVectorMath::Mul(_colliderStorage->boxStorage->scale[index], _transformStorage->scale[_transformIndex]) };

	float ixx{ (1.0f / 12.0f) * _mass * (size.y * size.y + size.z * size.z) };
	float iyy{ (1.0f / 12.0f) * _mass * (size.z * size.z + size.x * size.x) };
	float izz{ (1.0f / 12.0f) * _mass * (size.x * size.x + size.y * size.y) };

	return Matrix4x4{
		1 / ixx,0.0f,0.0f,0.0f,
		0.0f,1 / iyy,0.0f,0.0f,
		0.0f,0.0f,1 / izz,0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

Matrix4x4 RigidBodySystem::GenerateSphereInverseInertiaTensor(const uint32_t& _transformIndex, const ColliderID& _colliderID, float _mass, PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage)
{
	uint32_t index{ _colliderStorage->GetDenseIndex(_colliderID) };
	float radius{ _colliderStorage->sphereStorage->radius[index] * std::max(std::max(_transformStorage->scale[_transformIndex].x, _transformStorage->scale[_transformIndex].y), _transformStorage->scale[_transformIndex].z) };
	float i{ (2.0f / 5.0f) * _mass * radius * radius };

	return Matrix4x4{
		1 / i,0.0f,0.0f,0.0f,
		0.0f,1 / i,0.0f,0.0f,
		0.0f,0.0f,1 / i,0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}
