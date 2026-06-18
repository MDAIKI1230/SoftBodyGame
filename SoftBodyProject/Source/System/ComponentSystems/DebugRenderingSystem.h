#pragma once

#include "RenderingSystem.h"

class DebugRenderingSystem :public RenderingSystem
{
public:
	void Draw(IWorld* world) override;
};