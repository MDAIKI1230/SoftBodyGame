#include "PhysicsAPI.h"

#include "DistanceConstraintComponent.h"


DistanceConstraintComponent::DistanceConstraintComponent(EntityID _entity, int _handle) :
	ComponentBase{ _handle }
{
	id = PhysicsAPI::CreateDistanceConstraint(_entity, Vector3::ZERO, 0.0f);
}

DistanceConstraintComponent::DistanceConstraintComponent(EntityID _entity, int _handle, Vector3 _localOffset) :
	ComponentBase{ _handle }
{
	id = PhysicsAPI::CreateDistanceConstraint(_entity, _localOffset, 0.0f);
}

DistanceConstraintComponent::DistanceConstraintComponent(EntityID _entity, int _handle, float _distance) :
	ComponentBase{ _handle }
{
	id = PhysicsAPI::CreateDistanceConstraint(_entity, Vector3::ZERO, _distance);
}

DistanceConstraintComponent::DistanceConstraintComponent(EntityID _entity, int _handle, Vector3 _localOffset, float _distance) :
	ComponentBase{ _handle }
{
	id = PhysicsAPI::CreateDistanceConstraint(_entity, _localOffset, _distance);
}

void DistanceConstraintComponent::AddEndPoint(const EntityID& _entityID, const Vector3& _localOffset)
{
	PhysicsAPI::AddEndPoint(id, _entityID, _localOffset);
}

float DistanceConstraintComponent::GetDistance()
{
	return PhysicsAPI::GetDistance(id);
}

void DistanceConstraintComponent::SetDistance(float _distance)
{
	PhysicsAPI::SetDistance(id, _distance);
}
