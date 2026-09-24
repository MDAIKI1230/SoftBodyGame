#pragma once

#include "RenderingSystem.h"

class SpriteRenderingSystem :public RenderingSystem
{
public:
	void Draw(WorldStorage* _worldStorage, EventManager* _eventManager) override;
};