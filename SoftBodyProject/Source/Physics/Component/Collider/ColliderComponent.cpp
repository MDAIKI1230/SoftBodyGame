#include "PhysicsComponentAPI.h"

#include "ColliderComponent.h"

// フィルター取得
const CollisionFilter& ColliderComponent::GetFilter()
{
	return PhysicsComponentAPI::GetFilter(id);
}
// フィルター変更
void ColliderComponent::SetFilter(const CollisionFilter& _filter)
{
	PhysicsComponentAPI::SetFilter(id, _filter);
}

// Colliderが現在も有効か
bool ColliderComponent::IsValid() const
{
	return PhysicsComponentAPI::IsColliderAlive(id);
}

// Colliderが有効ならtrue
ColliderComponent::operator bool() const
{
	return IsValid();
}

// Colliderの種類取得
ColliderType ColliderComponent::GetType() const
{
	return PhysicsComponentAPI::GetColliderType(id);
}

// Sphereか
bool ColliderComponent::IsSphere() const
{
	return GetType() == ColliderType::SPHERE;
}

// Boxか
bool ColliderComponent::IsBox() const
{
	return GetType() == ColliderType::BOX;
}

// Capsuleか
bool ColliderComponent::IsCapsule() const
{
	return GetType() == ColliderType::CAPSULE;
}

// 同じColliderか比較
bool ColliderComponent::operator==(const ColliderComponent& _other) const
{
	return id == _other.id;
}
