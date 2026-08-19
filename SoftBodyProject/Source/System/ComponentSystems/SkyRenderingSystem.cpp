#include "ServiceLocator.h"

#include "TransformComponentStorage.h"
#include "CameraComponentStorage.h"

#include "SkyRenderingSystem.h"

void SkyRenderingSystem::Initialize()
{
#ifdef USE_RAYLIB
	sphereHandle = ServiceLocator::GetRenderer()->LoadModel("Res/Model/SkySphere/SkySphere.glb");
#else
	sphereHandle = ServiceLocator::GetRenderer()->LoadModel("Res/Model/SkySphere/SkySphere.mv1");
#endif // USE_RAYLIB
}

// 描画
void SkyRenderingSystem::Draw(WorldStorage* _worldStorage, EventManager* _eventManager)
{
	// Transformストレージ
	TransformComponentStorage* transformStorage{ static_cast<TransformComponentStorage*>(_worldStorage->GetStorage<TransformComponent>()) };
	// Cameraストレージ
	CameraComponentStorage* cameraStorage{ static_cast<CameraComponentStorage*>(_worldStorage->GetStorage<CameraComponent>()) };

	for (auto cameraEntity : cameraStorage->GetEntities())
	{
		const TransformComponent& trns{ transformStorage->Get(cameraEntity) };

		ServiceLocator::GetRenderer()->ModelSetMatrix(sphereHandle, trns.GetWorldMatrix());

		ServiceLocator::GetRenderer()->DrawModel(sphereHandle);
	}
}
