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

		uint32_t indexTrans{ _transformStorage->GetDenseIndex(transID) };

		Renderer::DrawSphereMesh(
			_transformStorage->GetPosition(indexTrans),
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

		uint32_t indexTrans{ _transformStorage->GetDenseIndex(transID) };

		Renderer::DrawBox(
			_transformStorage->GetWorldMatrix(indexTrans),
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

		uint32_t indexTrans{ _transformStorage->GetDenseIndex(transID) };

		// 軸の半分
		const Quaternion& rot{ _transformStorage->GetRotation(indexTrans) };
		const Vector3& pos{ _transformStorage->GetPosition(indexTrans) };
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
