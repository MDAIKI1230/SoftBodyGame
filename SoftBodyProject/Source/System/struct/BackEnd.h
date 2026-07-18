#pragma once


#include <memory>

#include "IRenderer.h"
#include "IInput.h"
#include "ISystem.h"

struct BackEnd
{
	std::unique_ptr<IRenderer> renderer;
	std::unique_ptr<IInput> input;
	std::unique_ptr<ISystem> system;
};
