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
	for (auto& bodyID : _bodyStorage->GetRigidBodyIDRange())
	{
		// 実データインデックスの取得
		// フラグ判定
		if (_bodyStorage->GetRigidBodyIsGravity(bodyID))
		{
			// 質量×重力加速度(Δtに離散化)を力に加算
			_bodyStorage->EditRigidBodyForce(bodyID) += _bodyStorage->GetRigidBodyGravity(bodyID) * ServiceLocator::GetTimeManager()->GetFixedDeltaTime() * _bodyStorage->GetRigidBodyMass(bodyID);
		}
	}
}

void RigidBodySystem::UpdatePosition(PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage)
{
	for (auto& bodyID : _bodyStorage->GetRigidBodyIDRange())
	{
		// 実データインデックスの取得
		PhysicsTransformID transformID{ _bodyStorage->GetTransformID(bodyID) };
		uint32_t transIndex{ _transformStorage->GetDenseIndex(transformID) };
		// 速度 + 加速度(力(Δt) * 質量の逆数)
		Vector3& velocity{ _bodyStorage->EditRigidBodyVelocity(bodyID) };
		
		velocity += BodyStorage::ApplyLinearInverseMass(
			_bodyStorage->GetRigidBodyForce(bodyID) * ServiceLocator::GetTimeManager()->GetFixedDeltaTime(),
			_bodyStorage->GetRigidBodyInverseMass(bodyID),
			_bodyStorage->GetRigidBodyPositionLock(bodyID));

		velocity = SIMDVectorMath::Mul(
			velocity,
			_bodyStorage->GetRigidBodyPositionLock(bodyID));

		// 位置保存
		_bodyStorage->SetRigidBodyPastPosition(bodyID, _transformStorage->GetPosition(transIndex));

		// 今の位置 + 速度
		_transformStorage->EditPosition(transIndex) += _bodyStorage->GetRigidBodyVelocity(bodyID) * ServiceLocator::GetTimeManager()->GetFixedDeltaTime();
	}
}

void RigidBodySystem::UpdateRotation(PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage)
{
	for (auto& bodyID : _bodyStorage->GetRigidBodyIDRange())
	{
		// 実データインデックスの取得
		PhysicsTransformID transformID{ _bodyStorage->GetTransformID(bodyID) };
		uint32_t transIndex{ _transformStorage->GetDenseIndex(transformID) };
		// 角速度＋ 角加速度(トルク×慣性テンソルの逆数)
		Vector3& angularVelocity{ _bodyStorage->EditRigidBodyAngularVelocity(bodyID) };

		Matrix4x4 rotMat{ MatGenerateFunc::Rotate(_transformStorage->GetRotation(transIndex)) };

		angularVelocity +=
			BodyStorage::ApplyAngularInverseInertia(
				rotMat * _bodyStorage->GetRigidBodyLocalInverseInertiaTensor(bodyID) * rotMat.Transposed(),
				_bodyStorage->GetRigidBodyTorque(bodyID) * ServiceLocator::GetTimeManager()->GetFixedDeltaTime(),
				_bodyStorage->GetRigidBodyRotationLock(bodyID));

		angularVelocity = SIMDVectorMath::Mul(
			angularVelocity,
			_bodyStorage->GetRigidBodyRotationLock(bodyID));

		// 角速度と慣性テンソルの逆行列からΔt分の四元数を作成
		// Δω
		Vector3 deltaAngularVelocity{ _bodyStorage->GetRigidBodyAngularVelocity(bodyID) * ServiceLocator::GetTimeManager()->GetFixedDeltaTime() };
		// Δωの四元数を作る
		Quaternion rotOmega{ Quaternion::AngleAxis(deltaAngularVelocity.Length(),deltaAngularVelocity) };

		// 姿勢保存
		_bodyStorage->SetRigidBodyPastRotation(bodyID, _transformStorage->GetRotation(transIndex));

		// 今の回転＋トルク(Δtに離散化)×慣性テンソルの逆行列
		_transformStorage->EditRotation(transIndex) = (rotOmega * _transformStorage->GetRotation(transIndex)).Normalized();
	}
}

void RigidBodySystem::UpdateInverseInertiaTensor(PhysicsTransformStorage* _transformStorage, BodyStorage* _bodyStorage, ColliderStorage* _colliderStorage)
{
	for (auto& bodyID:_bodyStorage->GetRigidBodyIDRange())
	{
		// 実データインデックスの取得
		PhysicsTransformID transformID{ _bodyStorage->GetTransformID(bodyID) };
		uint32_t transIndex{ _transformStorage->GetDenseIndex(transformID) };
		// ローカル慣性テンソルの計算
		if (_bodyStorage->GetRigidBodyLocalInertiaDiary(bodyID))
		{
			for (auto& colliderID : _colliderStorage->GetColliderIDFromTransformID(transformID))
			{
				
				switch (_colliderStorage->GetType(colliderID))
				{
				case ColliderType::SPHERE:
					_bodyStorage->SetRigidBodyLocalInverseInertiaTensor(
						bodyID,
						GenerateSphereInverseInertiaTensor(transIndex, colliderID, _bodyStorage->GetRigidBodyMass(bodyID), _transformStorage, _colliderStorage)
					);
					break;
				case ColliderType::BOX:
					_bodyStorage->SetRigidBodyLocalInverseInertiaTensor(
						bodyID,
						GenerateBoxInverseInertiaTensor(transIndex, colliderID, _bodyStorage->GetRigidBodyMass(bodyID), _transformStorage, _colliderStorage)
					);
					break;
				case ColliderType::CAPSULE:
					_bodyStorage->SetRigidBodyLocalInverseInertiaTensor(
						bodyID,
						GenerateCapsuleInverseInertiaTensor(transIndex, colliderID, _bodyStorage->GetRigidBodyMass(bodyID), _transformStorage, _colliderStorage)
					);
				}
			}
			// フラグを戻す
			_bodyStorage->LocalInertiaCalcSucces(bodyID);
		}
	}
}

void RigidBodySystem::End(BodyStorage* _bodyStorage, ColliderStorage* _colliderStorage)
{
	for (auto& bodyID : _bodyStorage->GetRigidBodyIDRange())
	{
		_bodyStorage->EditRigidBodyForce(bodyID) = Vector3::ZERO;
		_bodyStorage->EditRigidBodyTorque(bodyID) = Vector3::ZERO;

		// AABBのフラグを変更する
		if (_bodyStorage->GetRigidBodyVelocity(bodyID).LengthSqr() >= MathConstants::EPSILON ||
			_bodyStorage->GetRigidBodyAngularVelocity(bodyID).LengthSqr() >= MathConstants::EPSILON)
		{
			auto& colliders{ _colliderStorage->GetColliderIDFromTransformID(_bodyStorage->GetTransformID(bodyID)) };
			for (auto& colliderID : colliders)
			{
				_colliderStorage->EditAABBDiaryFlag(_colliderStorage->GetAABBIndex(colliderID)) |= AABBChangeDiaryFlag::TRANSFORM;
			}
		}
	}
}


Matrix4x4 RigidBodySystem::GenerateBoxInverseInertiaTensor(uint32_t _transformIndex, ColliderID _colliderID, float _mass, PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage)
{
	Vector3 size{ SIMDVectorMath::Mul(_colliderStorage->GetBoxColliderScale(_colliderID), _transformStorage->GetScale(_transformIndex)) };

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
	Vector3 scale{ _transformStorage->GetScale(_transformIndex) };
	float radius{ _colliderStorage->GetSphereColliderRadius(_colliderID) * std::max(std::max(scale.x, scale.y), scale.z) };
	float i{ (2.0f / 5.0f) * _mass * radius * radius };

	return Matrix4x4{
		1 / i,0.0f,0.0f,0.0f,
		0.0f,1 / i,0.0f,0.0f,
		0.0f,0.0f,1 / i,0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

Matrix4x4 RigidBodySystem::GenerateCapsuleInverseInertiaTensor(uint32_t _transformIndex, ColliderID _colliderID, float _mass, PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage)
{
	const Vector3& scale{ _transformStorage->GetScale(_transformIndex) };

	// カプセルはローカルY軸方向。
	// 非一様スケール時も断面を円として扱うため、
	// 半径にはX・Zの大きい方を使用する。
	const float height{ std::abs(_colliderStorage->GetCapsuleColliderHeight(_colliderID) * scale.y) };

	float radiusScale{ std::max(std::abs(scale.x), std::abs(scale.z)) };

	float radius{ std::abs(_colliderStorage->GetCapsuleColliderRadius(_colliderID)) * radiusScale };

	float radiusSqr{ radius * radius };

	// πは円柱と球の質量比を求める際に打ち消し合うので省略可能。
	float cylinderVolume{ radiusSqr * height };

	// 上下の半球を合わせると1個の球と同じ体積。
	float sphereVolume{ (4.0f / 3.0f) * radiusSqr * radius };

	float totalVolume{ cylinderVolume + sphereVolume };

	if (_mass <= MathConstants::EPSILON ||
		totalVolume <= MathConstants::EPSILON)
	{
		return Matrix4x4{
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}

	// 一様密度として、体積比から円柱部分と半球部分へ質量を分配。
	float cylinderMass{ _mass * cylinderVolume / totalVolume };

	// 上下2個の半球を合計した質量。
	float capsMass{ _mass - cylinderMass };

	// 円柱部分の慣性モーメント。
	// Y軸が円柱の長軸。
	float cylinderIxx{ (1.0f / 12.0f) * cylinderMass * (3.0f * radiusSqr + height * height) };

	float cylinderIyy{ 0.5f * cylinderMass * radiusSqr };

	// 各半球の重心は、平面部分から3r/8だけ外側にある。
	float capCenterOffset{ height * 0.5f + radius * (3.0f / 8.0f) };

	// 上下2個の半球を合わせた慣性モーメント。
	float capsIxx{ capsMass * ((83.0f / 320.0f) * radiusSqr + capCenterOffset * capCenterOffset) };

	float capsIyy{ (2.0f / 5.0f) * capsMass * radiusSqr };

	// X軸とZ軸は対称。
	float ixx{ cylinderIxx + capsIxx };
	float iyy{ cylinderIyy + capsIyy };
	float izz{ ixx };

	float inverseIxx{
		ixx > MathConstants::EPSILON ? 1.0f / ixx : 0.0f
	};

	float inverseIyy{
		iyy > MathConstants::EPSILON ? 1.0f / iyy : 0.0f
	};

	float inverseIzz{
		izz > MathConstants::EPSILON ? 1.0f / izz : 0.0f
	};

	return Matrix4x4{
		inverseIxx, 0.0f,       0.0f,       0.0f,
		0.0f,       inverseIyy, 0.0f,       0.0f,
		0.0f,       0.0f,       inverseIzz, 0.0f,
		0.0f,       0.0f,       0.0f,       1.0f
	};
}
