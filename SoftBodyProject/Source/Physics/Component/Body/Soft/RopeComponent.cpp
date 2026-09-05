#include "PhysicsComponentAPI.h"

#include "RopeComponent.h"

// コンストラクタ
RopeComponent::RopeComponent(EntityID _entity) :
	id{ PhysicsComponentAPI::CreateRope(_entity, RopeUpdateInfo{}) }
{
}

// 全体の長さ取得
float RopeComponent::GetLength()
{
	return PhysicsComponentAPI::GetLength(id);
}
// 全体の長さ変更
void RopeComponent::SetLength(float _length)
{
	PhysicsComponentAPI::SetLength(id, _length);
}
// 分割数取得
int RopeComponent::GetSegmentCount()
{
	return PhysicsComponentAPI::GetSegmentCount(id);
}
// 分割数変更
void RopeComponent::SetSegmentCount(int _segmentCount)
{
	PhysicsComponentAPI::SetSegmentCount(id, _segmentCount);
}
