#include "SphereColliderComponent.h"

Vector3 SphereColliderComponent::Support(const Vector3& dir) const
{
	return dir.Normalized() * r;
}