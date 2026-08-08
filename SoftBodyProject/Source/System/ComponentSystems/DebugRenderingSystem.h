#pragma once

#include "RenderingSystem.h"

#include "TransformComponentStorage.h"
#include "SphereColliderComponentStorage.h"
#include "BoxColliderComponentStorage.h"
#include "CapsuleColliderComponentStorage.h"

#ifdef _DEBUG
class DebugRenderingSystem :public RenderingSystem
{
public:
	// 描画
	void Draw(WorldStorage* _worldStorage, EventManager* _eventManager) override;

	// 球描画
	void DrawSphera(SphereColliderComponentStorage* _sphereStorage, TransformComponentStorage* _transformStorage);
	// Box描画
	void DrawBox(BoxColliderComponentStorage* _boxStorage, TransformComponentStorage* _transformStorage);
	// Capsule描画
	void DrawCapsule(CapsuleColliderComponentStorage* _capsuleStorage, TransformComponentStorage* _transformStorage);
};
#endif // DEBUG
