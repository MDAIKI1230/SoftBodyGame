#include "SphereColliderComponent.h"

Vector3 SphereColliderComponent::Support(const Vector3& dir)
{
	return dir.Normalized() * r;
}