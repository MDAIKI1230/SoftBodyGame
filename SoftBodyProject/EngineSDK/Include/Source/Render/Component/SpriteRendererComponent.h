#pragma once

#include "TextureHandle.h"

struct SpriteRendererComponent
{
public:
	SpriteRendererComponent(TextureHandle _handle);
	SpriteRendererComponent(const char* _path);

	TextureHandle GetHandle() const
	{
		return handle;
	}

	~SpriteRendererComponent() = default;
private:
	TextureHandle handle;
};
