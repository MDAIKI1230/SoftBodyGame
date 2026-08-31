#include "PhysicsComponentAPI.h"

#include "ClothComponent.h"

// コンストラクタ
ClothComponent::ClothComponent(EntityID _entity) :
	id{ PhysicsComponentAPI::CreateCloth(_entity, ClothUpdateInfo{}) }
{
}

// 横幅取得
float ClothComponent::GetWidth()
{
	return PhysicsComponentAPI::GetClothWidth(id);
}
// 横幅変更
void ClothComponent::SetWidth(float _width)
{
	PhysicsComponentAPI::SetClothWidth(id, _width);
}
// 縦幅取得
float ClothComponent::GetHeight()
{
	return PhysicsComponentAPI::GetClothHeight(id);
}
// 縦幅変更
void ClothComponent::SetHeight(float _height)
{
	PhysicsComponentAPI::SetClothHeight(id, _height);
}
// 横分割取得
int ClothComponent::GetRowCount()
{
	return PhysicsComponentAPI::GetRowCount(id);
}
// 横分割変更
void ClothComponent::SetRowCount(int _rowCount)
{
	PhysicsComponentAPI::SetRowCount(id, _rowCount);
}
// 縦分割取得
int ClothComponent::GetColumnCount()
{
	return PhysicsComponentAPI::GetColumnCount(id);
}
// 縦分割変更
void ClothComponent::SetColumnCount(int _columnCount)
{
	PhysicsComponentAPI::SetColumnCount(id, _columnCount);
}
