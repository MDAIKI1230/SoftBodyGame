#include "Renderer.h"

#include "ColliderDebugRenderingSystem.h"

#ifdef _DEBUG
void ColliderDebugRenderingSystem::Render(PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage)
{
	RenderSphera(_transformStorage, _colliderStorage);
	RenderBox(_transformStorage, _colliderStorage);
	RenderCapsule(_transformStorage, _colliderStorage);
}

void ColliderDebugRenderingSystem::RenderSphera(PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage)
{
	// すべての球を描画していく
	for (auto id : _colliderStorage->GetSphereColliderIDRange())
	{
		PhysicsTransformID transID{ _colliderStorage->GetTransformID(id) };

		Vector3 offset{ _transformStorage->GetRotation(transID).Rotate(_colliderStorage->GetOffsetPosition(id)) };

		Renderer::DrawSphereMesh(
			_transformStorage->GetPosition(transID) + offset,
			_colliderStorage->GetSphereColliderRadius(id),
			Color{ 0.0f,1.0f, 0.0f });
	}
}

void ColliderDebugRenderingSystem::RenderBox(PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage)
{
	// すべてのボックスを描画していく
	for (auto id : _colliderStorage->GetBoxColliderIDRange())
	{

		PhysicsTransformID transID{ _colliderStorage->GetTransformID(id) };

		Matrix4x4 offset{ MatGenerateFunc::TRS(
			_colliderStorage->GetBoxColliderOffsetPosition(id),
			_colliderStorage->GetBoxColliderOffsetRotation(id),
			Vector3::ONE
		) };

		Renderer::DrawBox(
			_transformStorage->GetWorldMatrix(transID) * offset,
			_colliderStorage->GetBoxColliderScale(id),
			Color{ 0.0f,1.0f, 0.0f });
	}
}

// Capsule描画
void ColliderDebugRenderingSystem::RenderCapsule(PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage)
{
	// すべてのカプセルを描画していく
	for (auto id : _colliderStorage->GetCapsuleColliderIDRange())
	{
		PhysicsTransformID transID{ _colliderStorage->GetTransformID(id) };

		// 軸の半分
		const Quaternion& rot{ _transformStorage->GetRotation(transID) * _colliderStorage->GetCapsuleColliderOffsetRotation(id) };
		Vector3 offset{ _transformStorage->GetRotation(transID).Rotate(_colliderStorage->GetOffsetPosition(id)) };
		const Vector3& pos{ _transformStorage->GetPosition(transID) + offset };
		Vector3 axisHalf{ rot.Rotate(Vector3::UP * _colliderStorage->GetCapsuleColliderHeight(id) / 2.0f) };

		// Capsuleの始点終点
		Vector3 start{ pos + axisHalf };
		Vector3 end{ pos - axisHalf };
		Renderer::DrawCapsule(
			start,
			end,
			_colliderStorage->GetCapsuleColliderRadius(id),
			Color{ 0.0f,1.0f, 0.0f });
	}
}

#endif // DEBUG