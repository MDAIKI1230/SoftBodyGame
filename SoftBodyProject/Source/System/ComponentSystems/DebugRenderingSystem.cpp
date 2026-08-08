#include "ServiceLocator.h"

#include "DebugRenderingSystem.h"

#ifdef _DEBUG
void DebugRenderingSystem::Draw(WorldStorage* _worldStorage, EventManager* _eventManager)
{
	// コライダーストレージ
	SphereColliderComponentStorage* sphereStorage{ static_cast<SphereColliderComponentStorage*>(_worldStorage->GetStorage<SphereColliderComponent>()) };
	BoxColliderComponentStorage* boxStorage{ static_cast<BoxColliderComponentStorage*>(_worldStorage->GetStorage<BoxColliderComponent>()) };
	CapsuleColliderComponentStorage* capsuleStorage{ static_cast<CapsuleColliderComponentStorage*>(_worldStorage->GetStorage<CapsuleColliderComponent>()) };
	// Transformストレージ
	TransformComponentStorage* transformStorage{ static_cast<TransformComponentStorage*>(_worldStorage->GetStorage<TransformComponent>()) };

	DrawSphera(sphereStorage, transformStorage);
	DrawBox(boxStorage, transformStorage);
	DrawCapsule(capsuleStorage, transformStorage);
}

void DebugRenderingSystem::DrawSphera(SphereColliderComponentStorage* _sphereStorage, TransformComponentStorage* _transformStorage)
{
	// すべての球を描画していく
	for (EntityID entity : _sphereStorage->GetEntities())
	{
		auto trans{ _transformStorage->TryGet(entity) };
		// Transformがあるかチェックないなら飛ばす
		if (trans == nullptr)
		{
			continue;
		}

		for (auto& sphere : _sphereStorage->GetAll(entity))
		{
			ServiceLocator::GetRenderer()->DrawSphereMesh(trans->GetPosition(), sphere.GetRadius(), sphere.GetColor());
		}
	}
}

void DebugRenderingSystem::DrawBox(BoxColliderComponentStorage* _boxStorage, TransformComponentStorage* _transformStorage)
{
	// すべての球を描画していく
	for (EntityID entity : _boxStorage->GetEntities())
	{
		auto trans{ _transformStorage->TryGet(entity) };
		// Transformがあるかチェックないなら飛ばす
		if (trans == nullptr)
		{
			continue;
		}

		for (auto& box : _boxStorage->GetAll(entity))
		{
			Vector3 scale{ box.GetWidth(),box.GetHeight(),box.GetDepth() };

			ServiceLocator::GetRenderer()->DrawBox(trans->GetWorldMatrix(), scale, box.GetColor());
		}
	}
}

// Capsule描画
void DebugRenderingSystem::DrawCapsule(CapsuleColliderComponentStorage* _capsuleStorage, TransformComponentStorage* _transformStorage)
{
	// すべての球を描画していく
	for (EntityID entity : _capsuleStorage->GetEntities())
	{
		auto trans{ _transformStorage->TryGet(entity) };
		// Transformがあるかチェックないなら飛ばす
		if (trans == nullptr)
		{
			continue;
		}

		for (auto& capsule : _capsuleStorage->GetAll(entity))
		{
			// 軸の半分
			Quaternion rot{ trans->GetRotation() };
			Vector3 position{ trans->GetPosition() };
			Vector3 axisHalf{ rot.Rotate(Vector3::UP * capsule.GetHeight() / 2.0f) };

			// Capsuleの始点終点
			Vector3 start{ position + axisHalf };
			Vector3 end{ position - axisHalf };
			ServiceLocator::GetRenderer()->DrawCapsule(start, end, capsule.GetRadius(), capsule.GetColor());
		}
	}
}

#endif // DEBUG
