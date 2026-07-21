#pragma once

#include "ComponentBase.h"

struct RendererComponent : public ComponentBase
{
public:
	RendererComponent(int _handle) :
		ComponentBase(_handle)
	{
	}
};
