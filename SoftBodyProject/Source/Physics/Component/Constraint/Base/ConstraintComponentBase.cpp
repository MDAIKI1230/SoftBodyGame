#include "PhysicsComponentAPI.h"

#include "ConstraintComponentBase.h"

// 自身のEndPoint取得
const EndPointFrame& ConstraintComponentBase::GetEndPoint()
{
	return PhysicsComponentAPI::GetEndPoint(id);
}

// 自身のEndPoint変更
void ConstraintComponentBase::SetEndPoint(EndPointFrame _endPoint)
{
	PhysicsComponentAPI::SetEndPoint(id, _endPoint);
}

// 対応点追加
void ConstraintComponentBase::AddEndPoint(const ObjectBase* _object, const Vector3& _localOffset, const Quaternion& _rotation)
{
	PhysicsComponentAPI::AddEndPoint(id, _object->GetID(), _localOffset, _rotation);
}

// 対応点削除
void ConstraintComponentBase::RemoveEndPoint(const ObjectBase* _object)
{
	PhysicsComponentAPI::RemoveEndPoint(id, _object->GetID());
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

// 拘束からEndPointすべて除外
void ConstraintComponentBase::RemoveEndPointOtherAll()
{
	PhysicsComponentAPI::RemoveEndPointOtherAll(id);
}