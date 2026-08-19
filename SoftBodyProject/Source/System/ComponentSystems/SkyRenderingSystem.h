#pragma once

#include "RenderingSystem.h"

class SkyRenderingSystem :public RenderingSystem
{
public:
	// 初期化
	void Initialize() override;
	// 描画
	void Draw(WorldStorage* _worldStorage, EventManager* _eventManager) override;
private:
	int sphereHandle{ -1 };
};
