#pragma once

struct RendererComponent
{
public:
	RendererComponent() = default;

	RendererComponent(int _handle) :
		handle(_handle)
	{
	}

	int GetHandle() { return handle; }
private:
	int handle{ -1 };
};