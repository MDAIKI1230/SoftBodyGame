#include "ServiceLocator.h"

#include "RigidBodySystem.h"


void RigidBodySystem::FixedUpdate(PhysicsTransformStorage* _transformStorage, RigidBodyStorage* _bodyStorage)
{
	// 各処理実行
	UpdateGravity(_bodyStorage);
	UpdatePosition(_bodyStorage, _transformStorage);
	UpdateInverseInertiaTensor(_bodyStorage, _transformStorage);
	UpdateRotation(_bodyStorage, _transformStorage);
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

void RigidBodySystem::UpdatePosition(RigidBodyStorage* _bodyStorage, PhysicsTransformStorage* _transformStorage)
{
	for (auto& bodyID : _bodyStorage->id)
	{
		// 実データインデックスの取得
		uint32_t bodyIndex{ _bodyStorage->GetDenseIndex(bodyID) };
		PhysicsTransformID transformID{ _bodyStorage->GetTransformID(bodyID) };
		uint32_t transIndex{ _transformStorage->GetDenseIndex(transformID) };
		// 速度 + 加速度(力(Δt) * 質量の逆数)
		_bodyStorage->velocity[bodyIndex] += _bodyStorage->force[bodyIndex] * ServiceLocator::GetTimeManager()->GetFixedDeltaTime() * _bodyStorage->inverseMass[bodyIndex];


		// 今の位置 + 速度
		_transformStorage->position[transIndex] += _bodyStorage->velocity[bodyIndex] * ServiceLocator::GetTimeManager()->GetFixedDeltaTime();
	}
}

void RigidBodySystem::UpdateRotation(RigidBodyStorage* _bodyStorage, PhysicsTransformStorage* _transformStorage)
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

		// 今の回転＋トルク(Δtに離散化)×慣性テンソルの逆行列
		_transformStorage->rotation[transIndex] *= rotOmega;
	}
}

void RigidBodySystem::UpdateInverseInertiaTensor(RigidBodyStorage* _bodyStorage, PhysicsTransformStorage* _transformStorage)
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
			// スケール行列取得
			Matrix4x4 scaleMat{ MatGenerateFunc::Scale(_transformStorage->scale[transIndex]) };
			_bodyStorage->localInverseInertiaTensor[bodyIndex] = scaleMat * Matrix4x4::Identity();
			// フラグを戻す
			_bodyStorage->localInertiaDirty[bodyIndex] = false;
		}
		// 回転行列取得
		Matrix4x4 rotMat{ MatGenerateFunc::Rotate(_transformStorage->rotation[transIndex]) };
		_bodyStorage->worldInverseInertiaTensor[bodyIndex] = rotMat * Matrix4x4::Identity() * rotMat.Transpose();
	}
}
