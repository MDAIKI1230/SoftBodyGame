#pragma once

#include "ModelHandle.h"

struct RendererComponent
{
public:
	RendererComponent(ModelHandle _handle) :
		handle{_handle}
	{
	}

	ModelHandle GetHandle()
	{
		return handle;
	}

private:
	ModelHandle handle;
};
