#include "PhysicsAPI.h"

#include "ClothComponent.h"

// コンストラクタ
ClothComponent::ClothComponent(EntityID _entity, int _handle) :
	ComponentBase{ _handle }
{
	id = PhysicsAPI::CreateCloth(_entity, ClothUpdateInfo{});
}

// 横幅取得
float ClothComponent::GetWidth()
{
	return PhysicsAPI::GetClothWidth(id);
}
// 横幅変更
void ClothComponent::SetWidth(float _width)
{
	PhysicsAPI::SetClothWidth(id, _width);
}
// 縦幅取得
float ClothComponent::GetHeight()
{
	return PhysicsAPI::GetClothHeight(id);
}
// 縦幅変更
void ClothComponent::SetHeight(float _height)
{
	PhysicsAPI::SetClothHeight(id, _height);
}
// 横分割取得
int ClothComponent::GetRowCount()
{
	return PhysicsAPI::GetRowCount(id);
}
// 横分割変更
void ClothComponent::SetRowCount(int _rowCount)
{
	PhysicsAPI::SetRowCount(id, _rowCount);
}
// 縦分割取得
int ClothComponent::GetColumnCount()
{
	return PhysicsAPI::GetColumnCount(id);
}
// 縦分割変更
void ClothComponent::SetColumnCount(int _columnCount)
{
	PhysicsAPI::SetColumnCount(id, _columnCount);
}
