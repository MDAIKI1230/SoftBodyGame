#include "RigidBodyComponentStorage.h"
#include "BoxColliderComponentStorage.h"

#include "PhysicsAPI.h"

void PhysicsAPI::AddForce(int _handle, const Vector3& _force)
{
	// ストレージ取得
	RigidBodyComponentStorage* bodyStorage{ static_cast<RigidBodyComponentStorage*>(world->GetStorage<RigidBodyComponent>()) };

	// 加算
	bodyStorage->force[_handle] += _force;
}
void PhysicsAPI::AddTorque(int _handle, const Vector3& _torque)
{
	// ストレージ取得
	RigidBodyComponentStorage* bodyStorage{ static_cast<RigidBodyComponentStorage*>(world->GetStorage<RigidBodyComponent>()) };

	// 加算
	bodyStorage->torque[_handle] += _torque;
}

const Vector3& PhysicsAPI::GetVelocity(int _handle)
{
	// ストレージ取得
	RigidBodyComponentStorage* bodyStorage{ static_cast<RigidBodyComponentStorage*>(world->GetStorage<RigidBodyComponent>()) };

	return bodyStorage->velocity[_handle];
}

void PhysicsAPI::SetVelocity(int _handle, const Vector3& _velocity)
{
	// ストレージ取得
	RigidBodyComponentStorage* bodyStorage{ static_cast<RigidBodyComponentStorage*>(world->GetStorage<RigidBodyComponent>()) };

	bodyStorage->velocity[_handle] = _velocity;
}

const Vector3& PhysicsAPI::GetAngularVelocity(int _handle)
{
	// ストレージ取得
	RigidBodyComponentStorage* bodyStorage{ static_cast<RigidBodyComponentStorage*>(world->GetStorage<RigidBodyComponent>()) };

	return bodyStorage->angularVelocity[_handle];
}

void PhysicsAPI::SetAngularVelocity(int _handle, const Vector3& _omega)
{
	// ストレージ取得
	RigidBodyComponentStorage* bodyStorage{ static_cast<RigidBodyComponentStorage*>(world->GetStorage<RigidBodyComponent>()) };

	bodyStorage->angularVelocity[_handle] = _omega;
}

float PhysicsAPI::GetMass(int _handle)
{
	// ストレージ取得
	RigidBodyComponentStorage* bodyStorage{ static_cast<RigidBodyComponentStorage*>(world->GetStorage<RigidBodyComponent>()) };

	return bodyStorage->mass[_handle];
}

void PhysicsAPI::SetMass(int _handle, float _mass)
{
	if (_mass <= 0)
	{
		return;
	}

	// ストレージ取得
	RigidBodyComponentStorage* bodyStorage{ static_cast<RigidBodyComponentStorage*>(world->GetStorage<RigidBodyComponent>()) };

	bodyStorage->mass[_handle] = _mass;
	bodyStorage->inverseMass[_handle] = 1 / _mass;
}

const Matrix4x4& PhysicsAPI::GetInertiaTensor(int _handle)
{
	// ストレージ取得
	RigidBodyComponentStorage* bodyStorage{ static_cast<RigidBodyComponentStorage*>(world->GetStorage<RigidBodyComponent>()) };

	return bodyStorage->inertiaTensor[_handle];
}

void PhysicsAPI::SetInertiaTensor(int _handle, Matrix4x4& _matrix)
{
	// ストレージ取得
	RigidBodyComponentStorage* bodyStorage{ static_cast<RigidBodyComponentStorage*>(world->GetStorage<RigidBodyComponent>()) };

	bodyStorage->inertiaTensor[_handle] = _matrix;
	bodyStorage->inverseInertiaTensor[_handle] = _matrix.Inversed();
}

bool PhysicsAPI::GetIsGravity(int _handle)
{
	// ストレージ取得
	RigidBodyComponentStorage* bodyStorage{ static_cast<RigidBodyComponentStorage*>(world->GetStorage<RigidBodyComponent>()) };

	return bodyStorage->isGravity[_handle];
}

void PhysicsAPI::SetIsGravity(int _handle, bool _isGravity)
{
	// ストレージ取得
	RigidBodyComponentStorage* bodyStorage{ static_cast<RigidBodyComponentStorage*>(world->GetStorage<RigidBodyComponent>()) };

	bodyStorage->isGravity[_handle] = _isGravity;
}

const Vector3& PhysicsAPI::GetGravity(int _handle)
{
	// ストレージ取得
	RigidBodyComponentStorage* bodyStorage{ static_cast<RigidBodyComponentStorage*>(world->GetStorage<RigidBodyComponent>()) };

	return bodyStorage->gravity[_handle];
}

void PhysicsAPI::SetGravity(int _handle, const Vector3& _gravity)
{
	// ストレージ取得
	RigidBodyComponentStorage* bodyStorage{ static_cast<RigidBodyComponentStorage*>(world->GetStorage<RigidBodyComponent>()) };

	bodyStorage->gravity[_handle] = _gravity;
}

// X方向の長さ取得
float PhysicsAPI::GetWidth(int _handle)
{
	// ストレージ取得
	BoxColliderComponentStorage* boxColliderStorage{ static_cast<BoxColliderComponentStorage*>(world->GetStorage<BoxColliderComponent>()) };

	return boxColliderStorage->width[_handle];
}

// X方向の長さ変更
void PhysicsAPI::SetWidth(int _handle, float _width)
{
	// ストレージ取得
	BoxColliderComponentStorage* boxColliderStorage{ static_cast<BoxColliderComponentStorage*>(world->GetStorage<BoxColliderComponent>()) };

	boxColliderStorage->width[_handle] = _width;
}

// Y方向の長さ取得
float PhysicsAPI::GetHeight(int _handle)
{
	// ストレージ取得
	BoxColliderComponentStorage* boxColliderStorage{ static_cast<BoxColliderComponentStorage*>(world->GetStorage<BoxColliderComponent>()) };

	return boxColliderStorage->height[_handle];
}

// Y方向の長さ変更
void PhysicsAPI::SetHeight(int _handle, float _height)
{
	// ストレージ取得
	BoxColliderComponentStorage* boxColliderStorage{ static_cast<BoxColliderComponentStorage*>(world->GetStorage<BoxColliderComponent>()) };

	boxColliderStorage->height[_handle] = _height;
}

// Z方向の長さ取得
float PhysicsAPI::GetDepth(int _handle)
{
	// ストレージ取得
	BoxColliderComponentStorage* boxColliderStorage{ static_cast<BoxColliderComponentStorage*>(world->GetStorage<BoxColliderComponent>()) };

	return boxColliderStorage->depth[_handle];
}

// Z方向の長さ変更
void PhysicsAPI::SetDepth(int _handle, float _depth)
{
	// ストレージ取得
	BoxColliderComponentStorage* boxColliderStorage{ static_cast<BoxColliderComponentStorage*>(world->GetStorage<BoxColliderComponent>()) };

	boxColliderStorage->depth[_handle] = _depth;
}

WorldStorage* PhysicsAPI::world{ nullptr };
