#pragma once


#include <memory>

#include "IRenderer.h"
#include "IInput.h"
#include "InputSystem.h"
#include "ISystem.h"
#include "IGPUConnecter.h"

struct BackEnd
{
	std::unique_ptr<IRenderer> renderer;
	std::unique_ptr<InputSystem> inputSystem;
	std::unique_ptr<ISystem> system;
	std::unique_ptr<IGPUConnecter> gpuConnecter;
};
