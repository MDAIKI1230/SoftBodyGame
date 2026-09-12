#include "PhysicsComponentAPI.h"

#include "DirectionEndPointConstraintComponentBase.h"

// 対応点追加
void DirectionEndPointConstraintComponentBase::AddEndPoint(EntityID _entityID, const Vector3& _localOffset, const Vector3& _localDirection)
{
	PhysicsComponentAPI::AddDirectionEndPoint(id, _entityID, _localOffset, _localDirection);
}

// 対応点削除
void DirectionEndPointConstraintComponentBase::RemoveEndPoint(EntityID _entityID)
{
	PhysicsComponentAPI::RemoveEndPoint(id, _entityID);
}