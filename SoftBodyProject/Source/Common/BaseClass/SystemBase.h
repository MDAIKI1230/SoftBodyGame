#pragma once

#include "IWorld.h"

class SystemBase
{
public:
	// 優先度取得
	int GetPriority() const { return priority; }
	// 仮想デストラクタ
	virtual ~SystemBase() = default;
protected:
	// 優先度
	int priority{ 0 };
};