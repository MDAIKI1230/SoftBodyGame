#include "PhysicsComponentAPI.h"

#include "ConstraintComponentBase.h"

// 自信のEndPoint取得
const EndPointFrame& ConstraintComponentBase::GetEndPoint()
{
	return PhysicsComponentAPI::GetEndPoint(id);
}

// 自信のEndPoint変更
void ConstraintComponentBase::SetEndPoint(const EndPointFrame& _endPoint)
{
	PhysicsComponentAPI::SetEndPoint(id, _endPoint);
}

// 対応点追加
void ConstraintComponentBase::AddEndPoint(EntityID _entityID, const Vector3& _localOffset, const Quaternion& _rotation)
{
	PhysicsComponentAPI::AddEndPoint(id, _entityID, _localOffset, _rotation);
}

// 対応点削除
void ConstraintComponentBase::RemoveEndPoint(EntityID _entityID)
{
	PhysicsComponentAPI::RemoveEndPoint(id, _entityID);
}

// Bodyを指定して対応点を追加
void ConstraintComponentBase::AddEndPoint(const RigidBodyComponent& _body, const Vector3& _localOffset, const Quaternion& _rotation)
{
	PhysicsComponentAPI::AddEndPoint(id, _body.GetID(), _localOffset, _rotation);
}

// Bodyを指定して対応点を削除
void ConstraintComponentBase::RemoveEndPoint(const RigidBodyComponent& _body)
{
	PhysicsComponentAPI::RemoveEndPoint(id, _body.GetID());
}