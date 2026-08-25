#pragma once

#include "ObjectBase.h"
#include "WorldStorage.h"

#include "TransformComponent.h"
#include "CameraComponent.h"

class Camera:public ObjectBase
{
public:
	Camera(WorldStorage* _world, EntityID _entity, const Vector3& _position, const Vector3& _target) :
		ObjectBase{ _world,_entity },
		target{ _target }
	{
		trans = AddComponent<TransformComponent>();
		trans->SetPosition(_position);
		AddComponent<CameraComponent>();
	}

	// 位置取得
	const Vector3& GetPos()const { return trans->GetPosition(); }
	// ターゲット位置取得
	const Vector3& GetTarget()const { return target; }

	// --- 更新系 ---

	void Update() override {}
	void FixedUpdate() override {}

	// --- 衝突系 ---

	void OnCollisionEnter() override {}
	void OnCollision() override {}
	void OnCollisionExit() override {}
private:
	// 見るオブジェクト
	Vector3 target{};

	TransformComponent* trans;
};
