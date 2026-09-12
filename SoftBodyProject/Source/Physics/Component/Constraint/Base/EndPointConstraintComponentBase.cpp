#include "PhysicsComponentAPI.h"

#include "EndPointConstraintComponentBase.h"

// 対応点追加
void EndPointConstraintComponentBase::AddEndPoint(EntityID _entityID, const Vector3& _localOffset)
{
	PhysicsComponentAPI::AddEndPoint(id, _entityID, _localOffset);
}

// 対応点削除
void EndPointConstraintComponentBase::RemoveEndPoint(EntityID _entityID)
{
	PhysicsComponentAPI::RemoveEndPoint(id, _entityID);
}
