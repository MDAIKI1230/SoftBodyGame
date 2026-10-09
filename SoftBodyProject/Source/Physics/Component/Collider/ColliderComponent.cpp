#include "PhysicsComponentAPI.h"

#include "ColliderComponent.h"

// フィルター取得
const CollisionFilter& ColliderComponent::GetFilter() const
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

bool ColliderComponent::HasCategory(uint32_t _categoryBits) const
{
	return IsValid() && GetFilter().HasCategory(_categoryBits);
}

bool ColliderComponent::HasAllCategories(uint32_t _categoryBits) const
{
	return IsValid() && GetFilter().HasAllCategories(_categoryBits);
}

bool ColliderComponent::HasGroup() const
{
	return IsValid() && GetFilter().HasGroup();
}

bool ColliderComponent::CompareGroup(uint32_t _groupID) const
{
	return IsValid() && GetFilter().CompareGroup(_groupID);
}

bool ColliderComponent::CompareGroup(const CollisionFilter& _other) const
{
	return IsValid() && GetFilter().CompareGroup(_other);
}

bool ColliderComponent::CompareGroup(const ColliderComponent& _other) const
{
	return IsValid() && _other.IsValid() && GetFilter().CompareGroup(_other.GetFilter());
}

bool ColliderComponent::CompareMember(const ColliderComponent& _other) const
{
	return IsValid() && _other.IsValid() && GetFilter().CompareMember(_other.GetFilter());
}

bool ColliderComponent::CompareMember(const CollisionFilter& _other) const
{
	return IsValid() && GetFilter().CompareMember(_other);
}

bool ColliderComponent::IsMemberIgnored(uint32_t _memberIndex) const
{
	return IsValid() && GetFilter().IsMemberIgnored(_memberIndex);
}

bool ColliderComponent::CanCollide(const ColliderComponent& _other) const
{
	return IsValid() && _other.IsValid() && GetFilter().CanCollide(_other.GetFilter());
}

void ColliderComponent::AddCategory(uint32_t _categoryBits)
{
	if (!IsValid())
	{
		return;
	}

	CollisionFilter filter{ GetFilter() };
	filter.AddCategory(_categoryBits);
	SetFilter(filter);
}

void ColliderComponent::RemoveCategory(uint32_t _categoryBits)
{
	if (!IsValid())
	{
		return;
	}

	CollisionFilter filter{ GetFilter() };
	filter.RemoveCategory(_categoryBits);
	SetFilter(filter);
}

void ColliderComponent::AllowCategory(uint32_t _categoryBits)
{
	if (!IsValid())
	{
		return;
	}

	CollisionFilter filter{ GetFilter() };
	filter.AllowCategory(_categoryBits);
	SetFilter(filter);
}

void ColliderComponent::IgnoreCategory(uint32_t _categoryBits)
{
	if (!IsValid())
	{
		return;
	}

	CollisionFilter filter{ GetFilter() };
	filter.IgnoreCategory(_categoryBits);
	SetFilter(filter);
}

bool ColliderComponent::IgnoreMember(uint32_t _memberIndex)
{
	if (!IsValid())
	{
		return false;
	}

	CollisionFilter filter{ GetFilter() };
	if (!filter.IgnoreMember(_memberIndex))
	{
		return false;
	}

	SetFilter(filter);
	return true;
}

bool ColliderComponent::AllowMember(uint32_t _memberIndex)
{
	if (!IsValid())
	{
		return false;
	}

	CollisionFilter filter{ GetFilter() };
	if (!filter.AllowMember(_memberIndex))
	{
		return false;
	}

	SetFilter(filter);
	return true;
}

void ColliderComponent::IgnoreAllMembers()
{
	if (!IsValid())
	{
		return;
	}

	CollisionFilter filter{ GetFilter() };
	filter.IgnoreAllMembers();
	SetFilter(filter);
}

void ColliderComponent::AllowAllMembers()
{
	if (!IsValid())
	{
		return;
	}

	CollisionFilter filter{ GetFilter() };
	filter.AllowAllMembers();
	SetFilter(filter);
}