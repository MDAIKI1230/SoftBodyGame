#pragma once

#include "MonoBehaviour.h"
#include "WorldStorage.h"

#include "TransformComponent.h"
#include "CameraComponent.h"
#include "CameraRigComponent.h"

class Camera:public MonoBehaviour
{
public:
	Camera(WorldStorage* _world, EntityID _entity) :
		MonoBehaviour{ _world,_entity }
	{
		trans = AddComponent<TransformComponent>();
		AddComponent<CameraComponent>();
		AddComponent<CameraRigComponent>();
	}

	// --- 更新系 ---

	void Update() override {}
	void FixedUpdate() override {}

	// --- 衝突系 ---

	void OnCollisionEnter(CollisionInfo _info) override {}
	void OnCollision(CollisionInfo _info) override {}
	void OnCollisionExit(CollisionInfo _info) override {}
private:
	TransformComponent* trans;
};
