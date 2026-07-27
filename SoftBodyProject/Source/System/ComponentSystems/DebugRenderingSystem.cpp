#include "ServiceLocator.h"

#include "DebugRenderingSystem.h"

#ifdef _DEBUG
void DebugRenderingSystem::Draw(WorldStorage* _worldStorage, EventManager* _eventManager)
{
	// コライダーストレージ
	SphereColliderComponentStorage* sphereStorage{ static_cast<SphereColliderComponentStorage*>(_worldStorage->GetStorage<SphereColliderComponent>()) };
	BoxColliderComponentStorage* boxStorage{ static_cast<BoxColliderComponentStorage*>(_worldStorage->GetStorage<BoxColliderComponent>()) };
	// Transformストレージ
	TransformComponentStorage* transformStorage{ static_cast<TransformComponentStorage*>(_worldStorage->GetStorage<TransformComponent>()) };

	DrawSphera(sphereStorage, transformStorage);
	DrawBox(boxStorage, transformStorage);
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

#endif // DEBUG
