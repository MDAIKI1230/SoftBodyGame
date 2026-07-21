#include "PhysicsAPI.h"

#include "PointConstraintComponent.h"

PointConstraintComponent::PointConstraintComponent(EntityID _entity, int _handle) :
	ComponentBase{ _handle }
{
	id = PhysicsAPI::CreatePointConstraint(_entity, Vector3::ZERO);
}

PointConstraintComponent::PointConstraintComponent(EntityID _entity, int _handle, Vector3 _localOffset):
	ComponentBase{ _handle }
{
	id = PhysicsAPI::CreatePointConstraint(_entity, _localOffset);
}

// 対応点追加
void PointConstraintComponent::AddEndPoint(const EntityID& _entityID, const Vector3& _localOffset)
{
	PhysicsAPI::AddEndPoint(id, _entityID, _localOffset);
}
