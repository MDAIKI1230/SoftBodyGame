#include "PhysicsAPI.h"

#include "PointConstraintComponent.h"

PointConstraintComponent::PointConstraintComponent(EntityID _entity, int _handle) :
	ComponentBase{ _handle }
{
	id = PhysicsAPI::CreatePointConstraint(_entity, Vector3::ZERO);
}

// 対応点追加
void PointConstraintComponent::AddEndPoint(EntityID& _entityID, Vector3& _localOffset)
{
	PhysicsAPI::AddEndPoint(id, _entityID, _localOffset);
}
