#pragma once


#include <memory>

#include "IRenderer.h"
#include "IInput.h"
#include "ISystem.h"
#include "IGPUConnecter.h"

struct BackEnd
{
	std::unique_ptr<IRenderer> renderer;
	std::unique_ptr<IInput> input;
	std::unique_ptr<ISystem> system;
	std::unique_ptr<IGPUConnecter> gpuConnecter;
};
