#pragma once
#include "ConstraintComponentBase.h"
#include "PhysicsComponentAPI.h"

// 自身のEndPoint取得
template<class T, bool ONE>
const EndPointFrame& ConstraintComponentBase<T, ONE>::GetEndPoint()
{
	return PhysicsComponentAPI::GetEndPoint<T>(id);
}

// 自身のEndPoint変更
template<class T, bool ONE>
void ConstraintComponentBase<T, ONE>::SetEndPoint(EndPointFrame _endPoint)
{
	PhysicsComponentAPI::SetEndPoint<T>(id, _endPoint);
}

// 相手EndPoint取得
template<class T, bool ONE>
const EndPointFrame& ConstraintComponentBase<T, ONE>::GetOtherEndPoint() requires (ONE)
{
	return PhysicsComponentAPI::GetOtherEndPoint<T>(id);
}
// 相手EndPointすべて取得
template<class T, bool ONE>
std::span<const EndPointFrame> ConstraintComponentBase<T, ONE>::GetOtherEndPoints() requires (!ONE)
{
	return PhysicsComponentAPI::GetOtherEndPoints<T>(id);
}

// 対応点追加
template<class T, bool ONE>
void ConstraintComponentBase<T, ONE>::AddEndPoint(const ObjectBase* _object, const Vector3& _localOffset, const Quaternion& _rotation)
{
	PhysicsComponentAPI::AddEndPoint<T>(id, _object->GetID(), _localOffset, _rotation);
}

// 対応点削除
template<class T, bool ONE>
void ConstraintComponentBase<T, ONE>::RemoveEndPoint(const ObjectBase* _object) requires (!ONE)
{
	PhysicsComponentAPI::RemoveEndPoint<T>(id, _object->GetID());
}

// 対応点削除
template<class T, bool ONE>
void ConstraintComponentBase<T, ONE>::RemoveEndPoint() requires (ONE)
{
	PhysicsComponentAPI::RemoveEndPoint<T>(id);
}

// Bodyを指定して対応点を追加
template<class T, bool ONE>
void ConstraintComponentBase<T, ONE>::AddEndPoint(const RigidBodyComponent& _body, const Vector3& _localOffset, const Quaternion& _rotation)
{
	PhysicsComponentAPI::AddEndPoint<T>(id, _body.GetID(), _localOffset, _rotation);
}

// Bodyを指定して対応点を削除
template<class T, bool ONE>
void ConstraintComponentBase<T, ONE>::RemoveEndPoint(const RigidBodyComponent& _body) requires (!ONE)
{
	PhysicsComponentAPI::RemoveEndPoint<T>(id, _body.GetID());
}

// 拘束からEndPointすべて除外
template<class T, bool ONE>
void ConstraintComponentBase<T, ONE>::RemoveEndPointOtherAll() requires (!ONE)
{
	PhysicsComponentAPI::RemoveEndPointOtherAll<T>(id);
}