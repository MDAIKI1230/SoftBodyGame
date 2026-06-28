#include "ServiceLocator.h"

#include "RigidBodySystem.h"


void RigidBodySystem::FixedUpdate(RigidBodyStorage* _bodyStorage)
{
	// 各処理実行
	UpdateGravity(_bodyStorage);
	UpdatePosition(_bodyStorage);
	UpdateRotation(_bodyStorage);
}

void RigidBodySystem::UpdateGravity(RigidBodyStorage* _bodyStorage)
{
	for (int i{ 0 }; i < _bodyStorage->id.size(); i++)
	{
		// フラグ判定
		if (_bodyStorage->isGravity[i])
		{
			// 質量×重力加速度(Δtに離散化)を力に加算
			_bodyStorage->force[i] += _bodyStorage->gravity[i] * ServiceLocator::GetTimeManager()->GetDeltaTime() * _bodyStorage->mass[i];
		}
	}
}

void RigidBodySystem::UpdatePosition(RigidBodyStorage* _bodyStorage)
{
	for (int i{ 0 }; i < _bodyStorage->id.size(); i++)
	{
		// 速度 + 加速度(力(Δt) * 質量の逆数)
		_bodyStorage->velocity[i] += _bodyStorage->force[i] * ServiceLocator::GetTimeManager()->GetDeltaTime() * _bodyStorage->inverseMass[i];


		// 今の位置 + 速度
		_bodyStorage->position[i] += _bodyStorage->velocity[i] * ServiceLocator::GetTimeManager()->GetDeltaTime();
	}
}

void RigidBodySystem::UpdateRotation(RigidBodyStorage* _bodyStorage)
{
	for (int i{ 0 }; i < _bodyStorage->id.size(); i++)
	{
		// 角速度＋ 角加速度(トルク×慣性テンソルの逆数)
		_bodyStorage->angularVelocity[i] += _bodyStorage->inverseInertiaTensor[i] * _bodyStorage->torque[i] * ServiceLocator::GetTimeManager()->GetDeltaTime();

		// 角速度と慣性テンソルの逆行列からΔt分の四元数を作成
		// Δω
		Vector3 deltaAngularVelocity{ _bodyStorage->angularVelocity[i] * ServiceLocator::GetTimeManager()->GetDeltaTime() };
		// Δωの四元数を作る
		Quaternion rotOmega{ Quaternion::AngleAxis(deltaAngularVelocity.Length(),deltaAngularVelocity.Normalized()) };

		// 今の回転＋トルク(Δtに離散化)×慣性テンソルの逆行列
		// _bodyStorage->rotation[i] *= rotOmega;
	}
}
