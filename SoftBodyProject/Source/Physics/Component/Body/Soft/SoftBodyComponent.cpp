#include "PhysicsAPI.h"

#include "SoftBodyComponent.h"

// コンストラクタ
SoftBodyComponent::SoftBodyComponent(EntityID _entity, int _handle) :
	ComponentBase{ _handle }
{
	id = PhysicsAPI::CreateSoftBody(_entity, SoftBodyUpdateInfo{});
}

// 横幅取得
float SoftBodyComponent::GetWidth()
{
	return PhysicsAPI::GetSoftBodyWidth(id);
}
// 横幅変更
void SoftBodyComponent::SetWidth(float _width)
{
	PhysicsAPI::SetSoftBodyWidth(id, _width);
}
// 縦幅取得
float SoftBodyComponent::GetHeight()
{
	return PhysicsAPI::GetSoftBodyHeight(id);
}
// 縦幅変更
void SoftBodyComponent::SetSoftBodyHeight(float _height)
{
	PhysicsAPI::SetSoftBodyHeight(id, _height);
}
// 奥行き取得
float SoftBodyComponent::GetSoftBodyDepth()
{
	return PhysicsAPI::GetSoftBodyDepth(id);
}
// 奥行き変更
void SoftBodyComponent::SetSoftBodyDepth(float _depth)
{
	PhysicsAPI::SetSoftBodyDepth(id, _depth);
}
// 横分割取得
int SoftBodyComponent::GetSegmentCountX()
{
	return PhysicsAPI::GetSegmentCountX(id);
}
// 横分割変更
void SoftBodyComponent::SetSegmentCountX(int _segmentCountX)
{
	PhysicsAPI::SetSegmentCountX(id, _segmentCountX);
}
// 縦分割取得
int SoftBodyComponent::GetSegmentCountY()
{
	return PhysicsAPI::GetSegmentCountY(id);
}
// 縦分割変更
void SoftBodyComponent::SetSegmentCountY(int _segmentCountY)
{
	PhysicsAPI::SetSegmentCountY(id, _segmentCountY);
}
// 奥行き分割取得
int SoftBodyComponent::GetSegmentCountZ()
{
	return PhysicsAPI::GetSegmentCountZ(id);
}
// 奥行き分割変更
void SoftBodyComponent::SetSegmentCountZ(int _segmentCountZ)
{
	PhysicsAPI::SetSegmentCountZ(id, _segmentCountZ);
}
