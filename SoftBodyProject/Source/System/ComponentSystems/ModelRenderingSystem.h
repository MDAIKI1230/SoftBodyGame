#pragma once

#include "RenderingSystem.h"

class ModelRenderingSystem:public RenderingSystem
{
public:
	void Draw(WorldStorage* _worldStorage, EventManager* _eventManager) override;
};
