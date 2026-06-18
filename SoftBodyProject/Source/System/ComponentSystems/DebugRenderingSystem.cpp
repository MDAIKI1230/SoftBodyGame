#include "ServiceLocator.h"

#include "TransformComponentStorage.h"
#include "SphereColliderComponentStorage.h"

#include "DebugRenderingSystem.h"

void DebugRenderingSystem::Draw(IWorld* world)
{
	// コライダーストレージ
	SphereColliderComponentStorage* sphereStorage{ static_cast<SphereColliderComponentStorage*>(world->GetStorage<SphereColliderComponent>()) };
	// Transformストレージ
	TransformComponentStorage* transformStorage{ static_cast<TransformComponentStorage*>(world->GetStorage<TransformComponent>()) };

	// 参照用
	TransformComponent trans{};

	// すべての球を描画していく
	for (int entity : *sphereStorage->GetEntities())
	{
		// Transformがあるかチェックないなら飛ばす
		if (!transformStorage->TryGet(entity, trans))
		{
			continue;
		}

		SphereColliderComponent* col{ sphereStorage->Get(entity) };

		ServiceLocator::GetRenderer()->DrawSphere(trans.GetPosition(), col->GetRadius());
	}
}