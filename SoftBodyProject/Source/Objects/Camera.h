#pragma once

#include "ObjectBase.h"
#include "WorldStorage.h"

#include "TransformComponent.h"
#include "CameraComponent.h"
#include "CameraRigComponent.h"

class Camera:public ObjectBase
{
public:
	Camera(WorldStorage* _world, EntityID _entity) :
		ObjectBase{ _world,_entity }
	{
		trans = AddComponent<TransformComponent>();
		AddComponent<CameraComponent>();
		AddComponent<CameraRigComponent>();
	}

	// --- 更新系 ---

	void Update() override {}
	void FixedUpdate() override {}

	// --- 衝突系 ---

	void OnCollisionEnter() override {}
	void OnCollision() override {}
	void OnCollisionExit() override {}
private:
	TransformComponent* trans;
};
