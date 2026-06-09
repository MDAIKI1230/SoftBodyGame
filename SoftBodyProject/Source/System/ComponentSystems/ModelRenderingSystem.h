#pragma once

#include "RenderingSystem.h"

class ModelRenderingSystem:public RenderingSystem
{
public:
	void Draw(IWorld* world) override;
};