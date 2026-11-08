#include "Renderer.h"

#include "CameraComponent.h"
#include "TransformComponent.h"

#include "CameraBindSystem.h"

/*
	FOVとか全然まだ適応しきれてない数値があるけど一旦デバッグやる用。
*/
void CameraBindSystem::LateUpdate(WorldStorage* _worldStorage, EventManager* _eventManager)
{
	ComponentStorageBase<CameraComponent>* cameraStorage{ _worldStorage->GetStorage<CameraComponent>() };
	ComponentStorageBase<TransformComponent>* transformStorage{ _worldStorage->GetStorage<TransformComponent>() };

	for (auto entity : cameraStorage->GetEntities())
	{
		const TransformComponent& trans{ transformStorage->Get(entity) };
		const CameraComponent& camera{ cameraStorage->Get(entity) };
		Renderer::SetCamera(MatGenerateFunc::InverseTRS(trans.GetPosition(), trans.GetRotation(), Vector3{ 1.0f,1.0f, 1.0f }), camera.GetNear(), camera.GetFar());
	}
}
