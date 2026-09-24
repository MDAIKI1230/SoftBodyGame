#pragma once

#include "TextureHandle.h"

struct SpriteRendererComponent
{
public:
	SpriteRendererComponent(TextureHandle _handle) :
		handle{ _handle }
	{
	}

	TextureHandle GetHandle() const
	{
		return handle;
	}

private:
	TextureHandle handle;
};
