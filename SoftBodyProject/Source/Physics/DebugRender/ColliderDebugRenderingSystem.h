#pragma once

#include "PhysicsTransformStorage.h"
#include "ColliderStorage.h"

#ifdef _DEBUG
class ColliderDebugRenderingSystem
{
public:
	// 描画
	void Render(PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage);

	// 球描画
	void RenderSphera(PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage);
	// Box描画
	void RenderBox(PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage);
	// Capsule描画
	void RenderCapsule(PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage);
};
#endif // DEBUG
