#include "PhysicsAPI.h"

#include "RopeComponent.h"

// コンストラクタ
RopeComponent::RopeComponent(EntityID _entity) :
	id{ PhysicsAPI::CreateRope(_entity, RopeUpdateInfo{}) }
{
}

// 全体の長さ取得
float RopeComponent::GetLength()
{
	return PhysicsAPI::GetLength(id);
}
// 全体の長さ変更
void RopeComponent::SetLength(float _length)
{
	PhysicsAPI::SetLength(id, _length);
}
// 分割数取得
int RopeComponent::GetSegmentCount()
{
	return PhysicsAPI::GetSegmentCount(id);
}
// 分割数変更
void RopeComponent::SetSegmentCount(int _segmentCount)
{
	PhysicsAPI::SetSegmentCount(id, _segmentCount);
}
