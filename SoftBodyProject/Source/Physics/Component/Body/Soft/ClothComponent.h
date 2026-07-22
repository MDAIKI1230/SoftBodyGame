#pragma once

#include "ComponentBase.h"

#include "EntityID.h"
#include "BodyID.h"

struct ClothComponent :public ComponentBase
{
public:
	// コンストラクタ
	ClothComponent(EntityID _entity, int _handle);

	// 横幅取得
	float GetWidth();
	// 横幅変更
	void SetWidth(float _width);
	// 縦幅取得
	float GetHeight();
	// 縦幅変更
	void SetHeight(float _height);
	// 横分割取得
	int GetRowCount();
	// 横分割変更
	void SetRowCount(int _rowCount);
	// 縦分割取得
	int GetColumnCount();
	// 縦分割変更
	void SetColumnCount(int _columnCount);
private:
	BodyID id;
};
