#pragma once

#include "CollisionManifoldBuffer.h"

#include "ColliderTags.h"

template<class A, class B>
struct ContactGenerator
{
    static bool Generate(ColliderID _colliderIDA, ColliderID _colliderIDB, PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage, CollisionManifoldBuffer* _manifoldBuffer) {
    }
};

template<ColliderTag::SphereTag, ColliderTag::SphereTag>
struct ContactGenerator
{
    static bool Generate(ColliderID _colliderIDA, ColliderID _colliderIDB, PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage, CollisionManifoldBuffer* _manifoldBuffer)
    {
        PhysicsTransformID transformIDA{ _colliderStorage->GetTransformID(_colliderIDA) };
        PhysicsTransformID transformIDB{ _colliderStorage->GetTransformID(_colliderIDB) };

        uint32_t transformIndexA{ _transformStorage->GetDenseIndex(transformIDA) };
        uint32_t transformIndexB{ _transformStorage->GetDenseIndex(transformIDB) };
        // 半径の合計
        float totalRadius{ 
            _colliderStorage->sphereStorage->radius[_colliderStorage->GetDenseIndex(_colliderIDA)] +
            _colliderStorage->sphereStorage->radius[_colliderStorage->GetDenseIndex(_colliderIDB)]
        };

        // 位置の距離
        float distSqr{ Vector3::Dot(_transformStorage->position[transformIDA],_transformStorage->position[transformIDB]) };

        if (distSqr > totalRadius * totalRadius)
        {
            return false;
        }

        Manifold manifold;

        manifold.colliderA = _colliderIDA;
        manifold.colliderB = _colliderIDB;

        manifold.normal = Vector3::Normalize(_transformStorage->position[transformIDA] - _transformStorage->position[transformIDB]);

        ContactPoint contactPoint;
        contactPoint.penetration = std::sqrtf(distSqr);
        contactPoint.position = _transformStorage->position[transformIDA] + manifold.normal * _colliderStorage->sphereStorage->radius[_colliderStorage->GetDenseIndex(_colliderIDA)];
        manifold.AddPoints(contactPoint);

        _manifoldBuffer->manifolds.push_back(manifold);

        return true;
    }
};

template<ColliderTag::BoxTag, ColliderTag::SphereTag>
struct ContactGenerator
{
    static bool Generate(ColliderID _colliderIDA, ColliderID _colliderIDB, PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage, CollisionManifoldBuffer* _manifoldBuffer) {
    }
};

template<ColliderTag::SphereTag, ColliderTag::BoxTag>
struct ContactGenerator
{
    static bool Generate(ColliderID _colliderIDA, ColliderID _colliderIDB, PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage, CollisionManifoldBuffer* _manifoldBuffer) {
    }
};

template<ColliderTag::BoxTag, ColliderTag::BoxTag>
struct ContactGenerator
{
    static bool Generate(ColliderID _colliderIDA, ColliderID _colliderIDB, PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage, CollisionManifoldBuffer* _manifoldBuffer) {
    }
};
