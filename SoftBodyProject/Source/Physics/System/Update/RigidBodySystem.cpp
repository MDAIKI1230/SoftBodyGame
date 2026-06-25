#include "ServiceLocator.h"

#include "RigidBodySystem.h"

RigidBodySystem::RigidBodySystem()
{
	priority = INT_MAX;
}

void RigidBodySystem::FixedUpdate(WorldStorage* _worldStorage, EventManager* _eventManager)
{
	// ストレージ取得
	RigidBodyComponentStorage* bodyStorage{ static_cast<RigidBodyComponentStorage*>(_worldStorage->GetStorage<RigidBodyComponent>()) };
	TransformComponentStorage* transformStorage{ static_cast<TransformComponentStorage*>(_worldStorage->GetStorage<TransformComponent>()) };

	// 各処理実行
	UpdateGravity(bodyStorage);
	UpdatePosition(bodyStorage, transformStorage);
	UpdateRotation(bodyStorage, transformStorage);
}

void RigidBodySystem::UpdateGravity(RigidBodyComponentStorage* _storage)
{
	for (int i{ 0 }; i < _storage->GetSize(); i++)
	{
		// フラグ判定
		if (_storage->isGravity[i])
		{
			// 質量×重力加速度(Δtに離散化)を力に加算
			_storage->force[i] += _storage->gravity[i] * ServiceLocator::GetTimeManager()->GetDeltaTime() * _storage->mass[i];
		}
	}
}

void RigidBodySystem::UpdatePosition(RigidBodyComponentStorage* _bodyStorage, TransformComponentStorage* _transformStorage)
{
	for (int entity : *_bodyStorage->GetEntities())
	{
		RigidBodyComponent* component{ _bodyStorage->Get(entity) };
		int handle{ component->GetHnadle() };

		// 速度 + 加速度(力(Δt) * 質量の逆数)
		_bodyStorage->velocity[handle] += _bodyStorage->force[handle] * ServiceLocator::GetTimeManager()->GetDeltaTime() * _bodyStorage->inverseMass[handle];


		// 今の位置 + 速度
		_bodyStorage->expectedPos[handle] = _transformStorage->Get(entity)->GetPosition() + _bodyStorage->velocity[handle] * ServiceLocator::GetTimeManager()->GetDeltaTime();
	}
}

void RigidBodySystem::UpdateRotation(RigidBodyComponentStorage* _bodyStorage, TransformComponentStorage* _transformStorage)
{
	for (int entity : *_bodyStorage->GetEntities())
	{
		RigidBodyComponent* component{ _bodyStorage->Get(entity) };
		int handle{ component->GetHnadle() };

		// 角速度＋ 角加速度(トルク×慣性テンソルの逆数)
		_bodyStorage->angularVelocity[handle] += _bodyStorage->inverseInertiaTensor[handle] * _bodyStorage->torque[handle] * ServiceLocator::GetTimeManager()->GetDeltaTime();

		// 角速度と慣性テンソルの逆行列からΔt分の四元数を作成
		// Δω
		Vector3 deltaAngularVelocity{ _bodyStorage->angularVelocity[handle] * ServiceLocator::GetTimeManager()->GetDeltaTime() };
		// Δωの四元数を作る
		Quaternion rotOmega{ Quaternion::AngleAxis(deltaAngularVelocity.Length(),deltaAngularVelocity.Normalized()) };

		// 今の回転＋トルク(Δtに離散化)×慣性テンソルの逆行列
		_bodyStorage->expectedRot[handle] = _transformStorage->Get(entity)->GetRotation() * rotOmega;
	}
}
