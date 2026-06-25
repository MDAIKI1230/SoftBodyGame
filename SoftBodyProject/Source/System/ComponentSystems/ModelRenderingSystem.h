#pragma once

#include "RenderingSystem.h"

class ModelRenderingSystem:public RenderingSystem
{
public:
	void Draw(WorldStorage* world) override;
};
