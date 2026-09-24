#pragma once

#include "RenderingSystem.h"

class SpriteRenderingSystem :public RenderingSystem
{
public:
	SpriteRenderingSystem()
	{
		priority = -100;
	}
	void Draw(WorldStorage* _worldStorage, EventManager* _eventManager) override;
};