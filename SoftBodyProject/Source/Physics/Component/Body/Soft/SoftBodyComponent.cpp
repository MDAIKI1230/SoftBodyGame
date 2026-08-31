#include "PhysicsComponentAPI.h"

#include "SoftBodyComponent.h"

// コンストラクタ
SoftBodyComponent::SoftBodyComponent(EntityID _entity) :
	id{ PhysicsComponentAPI::CreateSoftBody(_entity, SoftBodyUpdateInfo{}) }
{
}

// 横幅取得
float SoftBodyComponent::GetWidth()
{
	return PhysicsComponentAPI::GetSoftBodyWidth(id);
}
// 横幅変更
void SoftBodyComponent::SetWidth(float _width)
{
	PhysicsComponentAPI::SetSoftBodyWidth(id, _width);
}
// 縦幅取得
float SoftBodyComponent::GetHeight()
{
	return PhysicsComponentAPI::GetSoftBodyHeight(id);
}
// 縦幅変更
void SoftBodyComponent::SetSoftBodyHeight(float _height)
{
	PhysicsComponentAPI::SetSoftBodyHeight(id, _height);
}
// 奥行き取得
float SoftBodyComponent::GetSoftBodyDepth()
{
	return PhysicsComponentAPI::GetSoftBodyDepth(id);
}
// 奥行き変更
void SoftBodyComponent::SetSoftBodyDepth(float _depth)
{
	PhysicsComponentAPI::SetSoftBodyDepth(id, _depth);
}
// 横分割取得
int SoftBodyComponent::GetSegmentCountX()
{
	return PhysicsComponentAPI::GetSegmentCountX(id);
}
// 横分割変更
void SoftBodyComponent::SetSegmentCountX(int _segmentCountX)
{
	PhysicsComponentAPI::SetSegmentCountX(id, _segmentCountX);
}
// 縦分割取得
int SoftBodyComponent::GetSegmentCountY()
{
	return PhysicsComponentAPI::GetSegmentCountY(id);
}
// 縦分割変更
void SoftBodyComponent::SetSegmentCountY(int _segmentCountY)
{
	PhysicsComponentAPI::SetSegmentCountY(id, _segmentCountY);
}
// 奥行き分割取得
int SoftBodyComponent::GetSegmentCountZ()
{
	return PhysicsComponentAPI::GetSegmentCountZ(id);
}
// 奥行き分割変更
void SoftBodyComponent::SetSegmentCountZ(int _segmentCountZ)
{
	PhysicsComponentAPI::SetSegmentCountZ(id, _segmentCountZ);
}
