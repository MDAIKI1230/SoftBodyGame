#pragma once

#include "MonoBehaviour.h"

#include "TransformComponent.h"
#include "CameraComponent.h"
#include "CameraRigComponent.h"

#include "FieldMacros.h"

class Camera :public MonoBehaviour
{
public:
	Camera(WorldStorage* _world, EntityID _entity) :
		MonoBehaviour{ _world,_entity }
	{
		trans = GetComponent<TransformComponent>();
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

	~Camera()
	{
		if (main == this)
		{
			Camera::main = nullptr;
		}
	}

	MD_HAS_COMPONENT("Camera");
public:
	inline static Camera* main{ nullptr };

private:
	TransformComponent* trans;
};
