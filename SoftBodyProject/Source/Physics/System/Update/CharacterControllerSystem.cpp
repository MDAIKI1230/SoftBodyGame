#include <algorithm>

#include "TimeManager.h"
#include "../Query/PhysicsQuerySystem.h"

#include "CharacterControllerSystem.h"

// 更新関数
void CharacterControllerSystem::FixedUpdate(
	CharacterControllerStorage* _characterControllerStorage, PhysicsTransformStorage* _transformStorage,
	BodyStorage* _bodyStorage, ColliderStorage* _colliderStorage)
{
	UpdateGroundState(_characterControllerStorage, _transformStorage, _bodyStorage, _colliderStorage);
	UpdateMovement(_characterControllerStorage, _bodyStorage);
	UpdateJump(_characterControllerStorage, _bodyStorage);
}

// 地面との状態を更新する関数
void CharacterControllerSystem::UpdateGroundState(
	CharacterControllerStorage* _characterControllerStorage, PhysicsTransformStorage* _transformStorage,
	BodyStorage* _bodyStorage, ColliderStorage* _colliderStorage)
{
	for (auto id : _characterControllerStorage->GetIDRange())
	{
		uint32_t indexCC{ _characterControllerStorage->GetDenseIndex(id) };

		PhysicsTransformID transformID{ _characterControllerStorage->GetTransformID(indexCC) };

		uint32_t indexTransform{ _transformStorage->GetDenseIndex(transformID) };

		BodyID bodyID{ _characterControllerStorage->GetRigidBodyID(indexCC) };

		ColliderID colliderID{ _characterControllerStorage->GetCapsuleColliderID(indexCC) };

		const Vector3& position{ _transformStorage->GetPosition(indexTransform) };
		const Vector3& gravity{ _bodyStorage->GetRigidBodyGravity(bodyID) };

		// リセット
		// ヒット情報
		_characterControllerStorage->SetGroundNormal(indexCC, Vector3::ZERO);
		_characterControllerStorage->SetGroundPoint(indexCC, Vector3::ZERO);
		_characterControllerStorage->SetGroundDistance(indexCC, 0.0f);
		_characterControllerStorage->SetGroundColliderID(indexCC, ColliderID{});
		_characterControllerStorage->SetGroundState(indexCC, CharacterGroundState::AIRBORNE);

		if (gravity.LengthSqr() <= MathConstants::EPSILON * MathConstants::EPSILON)
		{
			continue;
		}

		// カプセルの高さ
		float height{ _colliderStorage->GetCapsuleColliderHeight(colliderID) };
		float radius{ _colliderStorage->GetCapsuleColliderRadius(colliderID) };
		// 滑る角度の判定値
		float minGroundDots{ _characterControllerStorage->GetMinGroundDot(indexCC) };
		// カプセルの下側。
		float capsuleBottom{ height * 0.5f + radius };

		RayCastQueryHitInfo hitInfo;
		Ray ray;
		ray.origin = position;
		ray.direction = gravity.Normalized();
		ray.maxDistance = capsuleBottom + _characterControllerStorage->GetGroundProbeDistance(indexCC);

		if (PhysicsQuerySystem::RayCastHit(ray, hitInfo, _colliderStorage, _transformStorage))
		{
			// ヒット情報を残す
			_characterControllerStorage->SetGroundNormal(indexCC, hitInfo.normal);
			_characterControllerStorage->SetGroundPoint(indexCC, hitInfo.point);
			_characterControllerStorage->SetGroundDistance(indexCC, hitInfo.distance - capsuleBottom);
			_characterControllerStorage->SetGroundColliderID(indexCC, hitInfo.colliderID);

			// 地面にいる判定
			_characterControllerStorage->SetGroundState(indexCC, CharacterGroundState::WALKABLE);

			// 滑るか判定
			if (Vector3::Dot(hitInfo.normal, -ray.direction) < minGroundDots)
			{
				_characterControllerStorage->SetGroundState(indexCC, CharacterGroundState::STEEP_SLOPE);
			}
		}
	}
}
// 移動更新関数
void CharacterControllerSystem::UpdateMovement(CharacterControllerStorage* _characterControllerStorage, BodyStorage* _bodyStorage)
{
	for (auto id : _characterControllerStorage->GetIDRange())
	{
		uint32_t indexCC{ _characterControllerStorage->GetDenseIndex(id) };

		switch (_characterControllerStorage->GetGroundState(indexCC))
		{
		case CharacterGroundState::AIRBORNE:
			UpdateAirboneState(indexCC, _characterControllerStorage, _bodyStorage, TimeManager::GetFixedDeltaTime());
			break;
		case CharacterGroundState::WALKABLE:
			UpdateWalkableState(indexCC, _characterControllerStorage, _bodyStorage, TimeManager::GetFixedDeltaTime());
			break;
		case CharacterGroundState::STEEP_SLOPE:
			UpdateSteepSlopeState(indexCC, _characterControllerStorage, _bodyStorage, TimeManager::GetFixedDeltaTime());
			break;
		default:
			break;
		}
	}
}
// ジャンプリクエスト処理関数
void CharacterControllerSystem::UpdateJump(CharacterControllerStorage* _characterControllerStorage, BodyStorage* _bodyStorage)
{
	for (auto id : _characterControllerStorage->GetIDRange())
	{
		uint32_t indexCC{ _characterControllerStorage->GetDenseIndex(id) };

		BodyID bodyID{ _characterControllerStorage->GetRigidBodyID(indexCC) };

		// ジャンプリクエストあり地面にいるなら地面の法線方向
		if (_characterControllerStorage->GetJumpRequest(indexCC))
		{
			_characterControllerStorage->SetJumpRequest(indexCC, false);
			if (_characterControllerStorage->GetGroundState(indexCC) == CharacterGroundState::WALKABLE)
			{
				const Vector3& gravity{ _bodyStorage->GetRigidBodyGravity(bodyID) };

				// 上を決める(重力がないならUP)
				Vector3 jumpDirection{ gravity.LengthSqr() >= MathConstants::EPSILON * MathConstants::EPSILON ? -gravity.Normalized() : Vector3::UP };

				Vector3& velocity{ _bodyStorage->EditRigidBodyVelocity(bodyID) };

				float jumpSpeed{ _characterControllerStorage->GetJumpSpeed(indexCC) };

				float currentJumpSpeed{ Vector3::Dot(velocity, jumpDirection) };

				if (currentJumpSpeed < jumpSpeed)
				{
					velocity += jumpDirection * (jumpSpeed - currentJumpSpeed);
				}

				_characterControllerStorage->SetGroundState(indexCC, CharacterGroundState::AIRBORNE);
			}
		}
	}
}

// 空中にいる時の移動更新関数
void CharacterControllerSystem::UpdateAirboneState(uint32_t _indexCC, CharacterControllerStorage* _characterControllerStorage, BodyStorage* _bodyStorage, float _deltaTime)
{
	// 入力
	const Vector3& input{ _characterControllerStorage->GetMoveInput(_indexCC) };
	// 加速度
	float acc{ _characterControllerStorage->GetAirAcceleration(_indexCC) };
	// 最大速度
	float maxSpeed{ _characterControllerStorage->GetMaxSpeed(_indexCC) };
	// BodyID
	BodyID bodyID{ _characterControllerStorage->GetRigidBodyID(_indexCC) };

	// 速度
	Vector3& velocity{ _bodyStorage->EditRigidBodyVelocity(bodyID) };
	// 重力
	const Vector3& gravity{ _bodyStorage->GetRigidBodyGravity(bodyID) };

	// 入力ないなら終了
	float inputLength{ input.Length() };

	if (inputLength < MathConstants::EPSILON)
	{
		return;
	}

	// 上方向を決める(重力が0になってる場合はUP)
	Vector3 up{ Vector3::UP };

	if (gravity.LengthSqr() >= MathConstants::EPSILON * MathConstants::EPSILON)
	{
		up = -gravity.Normalized();
	}

	// 入力を重力と垂直な方向に射影
	Vector3 projectedInput{ input - up * Vector3::Dot(input, up) };

	float projectedLength{ projectedInput.Length() };

	if (projectedLength < MathConstants::EPSILON)
	{
		return;
	}

	float inputAmount{ std::min(inputLength, 1.0f) };

	// 目標速度
	Vector3 targetVelocity{ projectedInput / projectedLength * maxSpeed * inputAmount };

	// 現在の速度を重力と垂直な方向に射影
	Vector3 projectedVelocity{ velocity - up * Vector3::Dot(velocity, up) };

	// 差分
	Vector3 deltaVelocity{ targetVelocity - projectedVelocity };

	float deltaLength{ deltaVelocity.Length() };

	// 最大加速度
	float maxAcc{ acc * _deltaTime };

	// 差分が最大加速度より大きいなら最大加速度にまで落とす。差分がほぼないなら何もしない
	if (deltaLength > maxAcc && deltaLength > MathConstants::EPSILON)
	{
		deltaVelocity *= maxAcc / deltaLength;
	}

	velocity += deltaVelocity;
}

// 歩ける状態の時の移動更新関数
void CharacterControllerSystem::UpdateWalkableState(uint32_t _indexCC, CharacterControllerStorage* _characterControllerStorage, BodyStorage* _bodyStorage, float _deltaTime)
{
	// 入力
	const Vector3& input{ _characterControllerStorage->GetMoveInput(_indexCC) };
	// 地面の法線
	const Vector3& groundNormal{ _characterControllerStorage->GetGroundNormal(_indexCC) };
	// 加速度
	float acc{ _characterControllerStorage->GetGroundAcceleration(_indexCC) };
	// 減速度
	float dec{ _characterControllerStorage->GetGroundDeceleration(_indexCC) };
	// 最大速度
	float maxSpeed{ _characterControllerStorage->GetMaxSpeed(_indexCC) };
	// BodyID
	BodyID bodyID{ _characterControllerStorage->GetRigidBodyID(_indexCC) };

	// 速度
	Vector3& velocity{ _bodyStorage->EditRigidBodyVelocity(bodyID) };
	// 力
	Vector3& force{ _bodyStorage->EditRigidBodyForce(bodyID) };
	// 重力
	const Vector3& gravity{ _bodyStorage->GetRigidBodyGravity(bodyID) };
	// 質量
	float mass{ _bodyStorage->GetRigidBodyMass(bodyID) };
	// 重力の有無
	bool isGravity{ _bodyStorage->GetRigidBodyIsGravity(bodyID) };

	// 入力がない場合は減速をする
	float inputLenSqr{ input.LengthSqr() };
	// 目標速度
	Vector3 targetVec{ Vector3::ZERO };
	// 入力の有無
	bool isInput{ inputLenSqr >= MathConstants::EPSILON * MathConstants::EPSILON };

	if (isInput)
	{
		// 入力の地面に対しての方向
		Vector3 inputDir{ input - groundNormal * Vector3::Dot(input, groundNormal) };

		if (inputDir.LengthSqr() >= MathConstants::EPSILON * MathConstants::EPSILON)
		{
			float inputAmount{ std::min(sqrtf(inputLenSqr), 1.0f) };

			targetVec = inputDir.Normalize() * maxSpeed * inputAmount;
		}
		else
		{
			isInput = false;
		}
	}

	// 地面に対しての移動
	Vector3 currentMoveVelocity{ velocity - groundNormal * Vector3::Dot(velocity, groundNormal) };
	// 必要なベクトル
	Vector3 deltaVelocity{ targetVec - currentMoveVelocity };
	// 必要なベクトルの長さ
	float deltaLen{ deltaVelocity.Length() };
	// 加速度
	float acceleration{ isInput ? acc : dec };
	// 最大加速度
	float maxAcc{ acceleration * _deltaTime };

	// 差分が最大加速度より大きいなら最大加速度にまで落とす。差分がほぼないなら何もしない
	if (deltaLen > maxAcc && deltaLen > MathConstants::EPSILON)
	{
		deltaVelocity *= maxAcc / deltaLen;
	}

	velocity += deltaVelocity;

	// 重力による滑りをなくす
	if (isGravity)
	{
		// 重力の法線方向成分
		Vector3 normalGravity{ groundNormal * Vector3::Dot(gravity, groundNormal) };

		// 重力の法線方向成分を打ち消した重力
		Vector3 slopeGravity{ gravity - normalGravity };

		force -= slopeGravity * mass;
	}
}

// 滑る地面の上にいる時の移動更新関数
void CharacterControllerSystem::UpdateSteepSlopeState(uint32_t _indexCC, CharacterControllerStorage* _characterControllerStorage, BodyStorage* _bodyStorage, float _deltaTime)
{
	// 地面の法線
	const Vector3& groundNormal{ _characterControllerStorage->GetGroundNormal(_indexCC) };
	// 滑り強めル加速度
	float slopeAcc{ _characterControllerStorage->GetSlopeAcceleration(_indexCC) };
	// BodyID
	BodyID bodyID{ _characterControllerStorage->GetRigidBodyID(_indexCC) };

	// 力
	Vector3& force{ _bodyStorage->EditRigidBodyForce(bodyID) };
	// 重力
	const Vector3& gravity{ _bodyStorage->GetRigidBodyGravity(bodyID) };
	// 質量
	float mass{ _bodyStorage->GetRigidBodyMass(bodyID) };
	// 重力の有無
	bool isGravity{ _bodyStorage->GetRigidBodyIsGravity(bodyID) };

	UpdateAirboneState(_indexCC, _characterControllerStorage, _bodyStorage, _deltaTime);

	// 重力による滑りを強める
	if (isGravity)
	{
		// 重力の法線方向成分
		Vector3 normalGravity{ groundNormal * Vector3::Dot(gravity, groundNormal) };

		// 重力の法線方向成分を打ち消した重力
		Vector3 slopeGravity{ gravity - normalGravity };

		if (slopeGravity.LengthSqr() >= MathConstants::EPSILON * MathConstants::EPSILON)
		{
			force += slopeGravity.Normalize() * slopeAcc * mass;
		}
	}
}
