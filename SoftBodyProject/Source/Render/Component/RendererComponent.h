#pragma once

#include "ModelHandle.h"

struct RendererComponent
{
public:
	RendererComponent(ModelHandle _handle) :
		handle{_handle}
	{
	}

	ModelHandle GetHandle() const
	{
		return handle;
	}

private:
	ModelHandle handle;
};
