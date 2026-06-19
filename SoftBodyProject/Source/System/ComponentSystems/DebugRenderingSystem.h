#pragma once

#include "RenderingSystem.h"

#ifdef _DEBUG
class DebugRenderingSystem :public RenderingSystem
{
public:
	void Draw(IWorld* world) override;
};
#endif // DEBUG
