#pragma once


#include <memory>

#include "IRenderer.h"
#include "ISystem.h"
#include "IGPUConnecter.h"
#include "ISound.h"

struct BackEnd
{
	std::unique_ptr<IRenderer> renderer;
	std::unique_ptr<ISystem> system;
	std::unique_ptr<IGPUConnecter> gpuConnecter;
	std::unique_ptr<ISound> sound;
};
