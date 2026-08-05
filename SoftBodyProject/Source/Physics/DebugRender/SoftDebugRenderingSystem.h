#pragma once

#include "GraphicsShaderHandle.h"

class SoftDebugRenderingSystem
{
public:
	SoftDebugRenderingSystem() = default;
	void Initialize();
	void Render();
private:
	GraphicsShaderHandle handle;
};
