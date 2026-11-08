#pragma once

#include "RenderingSystem.h"

class FrameRenderingSystem :public RenderingSystem
{
public:
	void Draw(WorldStorage* _worldStorage, EventManager* _eventManager) override;
};
