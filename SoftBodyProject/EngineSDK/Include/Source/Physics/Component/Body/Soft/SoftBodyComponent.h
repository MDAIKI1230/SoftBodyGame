#pragma once

#include "EntityID.h"
#include "BodyID.h"

struct SoftBodyComponent
{
public:
	// コンストラクタ
	SoftBodyComponent(EntityID _entity);

	// 横幅取得
	float GetWidth();
	// 横幅変更
	void SetWidth(float _width);
	// 縦幅取得
	float GetHeight();
	// 縦幅変更
	void SetSoftBodyHeight(float _height);
	// 奥行き取得
	float GetSoftBodyDepth();
	// 奥行き変更
	void SetSoftBodyDepth(float _depth);
	// 横分割取得
	int GetSegmentCountX();
	// 横分割変更
	void SetSegmentCountX(int _segmentCountX);
	// 縦分割取得
	int GetSegmentCountY();
	// 縦分割変更
	void SetSegmentCountY(int _segmentCountY);
	// 奥行き分割取得
	int GetSegmentCountZ();
	// 奥行き分割変更
	void SetSegmentCountZ(int _segmentCountZ);

	// ID取得
	BodyID GetID() const { return id; }
private:
	BodyID id;
};
