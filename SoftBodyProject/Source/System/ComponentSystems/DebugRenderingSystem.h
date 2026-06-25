#pragma once

#include "RenderingSystem.h"

#ifdef _DEBUG
class DebugRenderingSystem :public RenderingSystem
{
public:
	void Draw(WorldStorage* _worldStorage, EventManager* _eventManager) override;
};
#endif // DEBUG
