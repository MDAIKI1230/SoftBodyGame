#include "ServiceLocator.h"

#include "RigidBodySystem.h"


void RigidBodySystem::FixedUpdate(PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage, ColliderStorage* _colliderStorage)
{
	// 各処理実行
	UpdateGravity(_bodyStorage);
	UpdatePosition(_transformStorage, _bodyStorage);
	UpdateInverseInertiaTensor(_transformStorage, _bodyStorage, _colliderStorage);
	UpdateRotation(_transformStorage, _bodyStorage);
	End(_bodyStorage, _colliderStorage);
}

void RigidBodySystem::UpdateGravity(BodyStorage* _bodyStorage)
{
	for (auto& bodyID : _bodyStorage->rigidBodyStorage->id)
	{
		// 実データインデックスの取得
		uint32_t bodyIndex{ _bodyStorage->GetDenseIndex(bodyID) };
		// フラグ判定
		if (_bodyStorage->rigidBodyStorage->isGravity[bodyIndex])
		{
			// 質量×重力加速度(Δtに離散化)を力に加算
			_bodyStorage->rigidBodyStorage->force[bodyIndex] += _bodyStorage->rigidBodyStorage->gravity[bodyIndex] * ServiceLocator::GetTimeManager()->GetFixedDeltaTime() * _bodyStorage->rigidBodyStorage->mass[bodyIndex];
		}
	}
}

void RigidBodySystem::UpdatePosition(PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage)
{
	for (auto& bodyID : _bodyStorage->rigidBodyStorage->id)
	{
		// 実データインデックスの取得
		uint32_t bodyIndex{ _bodyStorage->GetDenseIndex(bodyID) };
		PhysicsTransformID transformID{ _bodyStorage->GetTransformID(bodyID) };
		uint32_t transIndex{ _transformStorage->GetDenseIndex(transformID) };
		// 速度 + 加速度(力(Δt) * 質量の逆数)
		_bodyStorage->rigidBodyStorage->velocity[bodyIndex] += _bodyStorage->rigidBodyStorage->force[bodyIndex] * ServiceLocator::GetTimeManager()->GetFixedDeltaTime() * _bodyStorage->rigidBodyStorage->inverseMass[bodyIndex];

		// 位置保存
		_bodyStorage->rigidBodyStorage->pastPos[bodyIndex] = _transformStorage->GetPosition(transIndex);

		// 今の位置 + 速度
		_transformStorage->EditPosition(transIndex) += _bodyStorage->rigidBodyStorage->velocity[bodyIndex] * ServiceLocator::GetTimeManager()->GetFixedDeltaTime();
	}
}

void RigidBodySystem::UpdateRotation(PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage)
{
	for (auto& bodyID : _bodyStorage->rigidBodyStorage->id)
	{
		// 実データインデックスの取得
		uint32_t bodyIndex{ _bodyStorage->GetDenseIndex(bodyID) };
		PhysicsTransformID transformID{ _bodyStorage->GetTransformID(bodyID) };
		uint32_t transIndex{ _transformStorage->GetDenseIndex(transformID) };
		// 角速度＋ 角加速度(トルク×慣性テンソルの逆数)
		_bodyStorage->rigidBodyStorage->angularVelocity[bodyIndex] += _bodyStorage->rigidBodyStorage->worldInverseInertiaTensor[bodyIndex] * _bodyStorage->rigidBodyStorage->torque[bodyIndex] * ServiceLocator::GetTimeManager()->GetFixedDeltaTime();

		// 角速度と慣性テンソルの逆行列からΔt分の四元数を作成
		// Δω
		Vector3 deltaAngularVelocity{ _bodyStorage->rigidBodyStorage->angularVelocity[bodyIndex] * ServiceLocator::GetTimeManager()->GetFixedDeltaTime() };
		// Δωの四元数を作る
		Quaternion rotOmega{ Quaternion::AngleAxis(deltaAngularVelocity.Length(),deltaAngularVelocity) };

		// 姿勢保存
		_bodyStorage->rigidBodyStorage->pastRot[bodyIndex] = _transformStorage->rotation[transIndex];

		// 今の回転＋トルク(Δtに離散化)×慣性テンソルの逆行列
		_transformStorage->rotation[transIndex] = (rotOmega * _transformStorage->rotation[transIndex]).Normalized();
	}
}

void RigidBodySystem::UpdateInverseInertiaTensor(PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage, ColliderStorage* _colliderStorage)
{
	for (auto& bodyID:_bodyStorage->rigidBodyStorage->id)
	{
		// 実データインデックスの取得
		uint32_t bodyIndex{ _bodyStorage->GetDenseIndex(bodyID) };
		PhysicsTransformID transformID{ _bodyStorage->GetTransformID(bodyID) };
		uint32_t transIndex{ _transformStorage->GetDenseIndex(transformID) };
		// ローカル慣性テンソルの計算
		if (_bodyStorage->rigidBodyStorage->localInertiaDirty[bodyIndex] == true)
		{
			for (auto& colliderID : _colliderStorage->GetColliderIDFromTransformID(transformID))
			{
				
				switch (_colliderStorage->GetType(colliderID))
				{
				case ColliderType::SPHERE:
					_bodyStorage->rigidBodyStorage->localInverseInertiaTensor[bodyIndex] = GenerateSphereInverseInertiaTensor(transIndex, colliderID, _bodyStorage->rigidBodyStorage->mass[bodyIndex], _transformStorage, _colliderStorage);
					break;
				case ColliderType::BOX:
					_bodyStorage->rigidBodyStorage->localInverseInertiaTensor[bodyIndex] = GenerateBoxInverseInertiaTensor(transIndex, colliderID, _bodyStorage->rigidBodyStorage->mass[bodyIndex], _transformStorage, _colliderStorage);
					break;
				}
			}
			// フラグを戻す
			_bodyStorage->rigidBodyStorage->localInertiaDirty[bodyIndex] = false;
		}
		// 回転行列取得
		Matrix4x4 rotMat{ MatGenerateFunc::Rotate(_transformStorage->rotation[transIndex]) };
		_bodyStorage->rigidBodyStorage->worldInverseInertiaTensor[bodyIndex] = rotMat * _bodyStorage->rigidBodyStorage->localInverseInertiaTensor[bodyIndex] * rotMat.Transposed();
	}
}

void RigidBodySystem::End(BodyStorage* _bodyStorage, ColliderStorage* _colliderStorage)
{
	for (auto& bodyID : _bodyStorage->rigidBodyStorage->id)
	{
		// 実データインデックスの取得
		uint32_t bodyIndex{ _bodyStorage->GetDenseIndex(bodyID) };
		_bodyStorage->rigidBodyStorage->force[bodyIndex] = Vector3::ZERO;
		_bodyStorage->rigidBodyStorage->torque[bodyIndex] = Vector3::ZERO;

		// AABBのフラグを変更する
		if (_bodyStorage->rigidBodyStorage->velocity[bodyIndex].LengthSqr() >= MathConstants::EPSILON ||
			_bodyStorage->rigidBodyStorage->angularVelocity[bodyIndex].LengthSqr() >= MathConstants::EPSILON)
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


Matrix4x4 RigidBodySystem::GenerateBoxInverseInertiaTensor(uint32_t _transformIndex, ColliderID _colliderID, float _mass, PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage)
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

Matrix4x4 RigidBodySystem::GenerateSphereInverseInertiaTensor(uint32_t _transformIndex, ColliderID _colliderID, float _mass, PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage)
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
