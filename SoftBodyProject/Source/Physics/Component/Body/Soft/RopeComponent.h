#pragma once

#include "ComponentBase.h"

#include "EntityID.h"
#include "BodyID.h"

struct RopeComponent :public ComponentBase
{
public:
	// コンストラクタ
	RopeComponent(EntityID& _entity, int _handle);

	// 全体の長さ取得
	float GetLength();
	// 全体の長さ変更
	void SetLength(float _length);
	// 分割数取得
	int GetSegmentCount();
	// 分割数変更
	void SetSegmentCount(int _segmentCount);
private:
	BodyID id;
};
