#pragma once

#include "ColliderTags.h"

template<class A, class B>
struct ContactGenerator
{
    static bool Generate(ColliderID _colliderIDA, ColliderID _colliderIDB, PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage) {
    }
};

template<ColliderTag::SphereTag, ColliderTag::SphereTag>
struct ContactGenerator
{
    static bool Generate(ColliderID _colliderIDA, ColliderID _colliderIDB, PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage) {
    }
};

template<ColliderTag::BoxTag, ColliderTag::SphereTag>
struct ContactGenerator
{
    static bool Generate(ColliderID _colliderIDA, ColliderID _colliderIDB, PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage) {
    }
};

template<ColliderTag::SphereTag, ColliderTag::BoxTag>
struct ContactGenerator
{
    static bool Generate(ColliderID _colliderIDA, ColliderID _colliderIDB, PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage) {
    }
};

template<ColliderTag::BoxTag, ColliderTag::BoxTag>
struct ContactGenerator
{
    static bool Generate(ColliderID _colliderIDA, ColliderID _colliderIDB, PhysicsTransformStorage* _transformStorage, ColliderStorage* _colliderStorage) {
    }
};
