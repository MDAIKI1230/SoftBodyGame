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
}

void DebugRenderingSystem::DrawSphera(SphereColliderComponentStorage* _sphereStorage, TransformComponentStorage* _transformStorage)
{
	// 参照用
	TransformComponent trans{};

	// すべての球を描画していく
	for (int entity : *_sphereStorage->GetEntities())
	{
		// Transformがあるかチェックないなら飛ばす
		if (!_transformStorage->TryGet(entity, trans))
		{
			continue;
		}

		SphereColliderComponent* col{ _sphereStorage->Get(entity) };

		ServiceLocator::GetRenderer()->DrawSphere(trans.GetPosition(), col->GetRadius(), col->GetColor());
	}
}

void DebugRenderingSystem::DrawBox(BoxColliderComponentStorage* _boxStorage, TransformComponentStorage* _transformStorage)
{

}

#endif // DEBUG
