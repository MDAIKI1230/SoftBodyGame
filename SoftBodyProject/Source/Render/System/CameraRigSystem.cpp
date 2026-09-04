#include <algorithm>
#include <cmath>

#include "PhysicsAPI.h"

#include "ServiceLocator.h"

#include "CameraRigSystem.h"

void CameraRigSystem::LateUpdate(WorldStorage* _worldStorage, EventManager* _eventManager)
{
	ComponentStorageBase<CameraRigComponent>* cameraRigStorage{ _worldStorage->GetStorage<CameraRigComponent>() };
	ComponentStorageBase<TransformComponent>* transformStorage{ _worldStorage->GetStorage<TransformComponent>() };

	for (auto entity : cameraRigStorage->GetEntities())
	{
		CameraRigComponent& cameraRig{ cameraRigStorage->Edit(entity) };
		TransformComponent& trans{ transformStorage->Edit(entity) };

		const TransformComponent* targetTrans{ transformStorage->TryGet(cameraRig.GetFollowTarget()) };
		if (targetTrans == nullptr)
		{
			cameraRig.ClearLookDelta();
			continue;
		}

		switch (cameraRig.GetMode())
		{
		case CameraMode::FPS:
			UpdateFPS(cameraRig, trans, targetTrans);
			break;
		case CameraMode::TPS:
			UpdateTPS(cameraRig, trans, targetTrans);
			break;
		default:
			break;
		}

		cameraRig.ClearLookDelta();
	}
}

void CameraRigSystem::UpdateFPS(CameraRigComponent& _cameraRig, TransformComponent& _trans, const TransformComponent* _targetTrans)
{
	// 位置をターゲットの一＋オフセットに合わせる。
	const Vector3& targetPos{ _targetTrans->GetPosition() };

	Vector3 offset{ _targetTrans->GetRotation().Rotate(_cameraRig.GetFirstPersonOffset()) };

	_trans.SetPosition(targetPos + offset);

	// ピッチとヨーを適応する
	const Vector2& lookDelta{ _cameraRig.GetLookDelta() };

	_cameraRig.SetYaw(_cameraRig.GetYaw() + lookDelta.x);
	float pitch{ std::clamp(_cameraRig.GetPitch() + lookDelta.y,_cameraRig.GetMinPitch(),_cameraRig.GetMaxPitch()) };
	_cameraRig.SetPitch(pitch);

	_trans.SetRotation(Quaternion::Euler(pitch, _cameraRig.GetYaw(), 0.0f));
}

void CameraRigSystem::UpdateTPS(CameraRigComponent& _cameraRig, TransformComponent& _trans, const TransformComponent* _targetTrans)
{
	// 基準点を求める
	Vector3 pivot{ _targetTrans->GetPosition() };

	Vector3 pivotOffset{ _targetTrans->GetRotation().Rotate(_cameraRig.GetPivotOffset()) };

	pivot += pivotOffset;

	// 基準点から離して,目標位置を決める
	const Vector2& lookDelta{ _cameraRig.GetLookDelta() };

	_cameraRig.SetYaw(_cameraRig.GetYaw() + lookDelta.x);
	float pitch{ std::clamp(_cameraRig.GetPitch() + lookDelta.y,_cameraRig.GetMinPitch(),_cameraRig.GetMaxPitch()) };
	_cameraRig.SetPitch(pitch);

	Quaternion rotation{ Quaternion::Euler(pitch, _cameraRig.GetYaw(), 0.0f) };

	Vector3 targetPos{ pivot + rotation.Rotate(_cameraRig.GetShoulderOffset() - (Vector3::FORWARD * _cameraRig.GetDistance())) };

	// 壁の判定をして壁とぶつかったらぶつかった位置に
	Ray ray;
	ray.origin = pivot;
	ray.direction = (targetPos - pivot).Normalize();
	ray.maxDistance = _cameraRig.GetDistance();
	RayCastHitInfo hitInfo;

	if (PhysicsAPI::RayCastHit(ray, hitInfo))
	{
		targetPos = hitInfo.point;
	}

	if (_cameraRig.GetPositionDamping() <= MathConstants::EPSILON)
	{
		_trans.SetPosition(targetPos);
	}
	else
	{
		// 現在との差分から、今回の移動量を決める
		Vector3 delta{ targetPos - _trans.GetPosition() };

		float t{ 1.0f - 
			std::exp(-ServiceLocator::GetTimeManager()->GetDeltaTime() / _cameraRig.GetPositionDamping()) };

		_trans.SetPosition(_trans.GetPosition() + delta * t);
	}

	_trans.SetRotation(rotation);
}
