#pragma once

struct RendererComponent
{
public:
	RendererComponent(int _handle) :
		handle{_handle}
	{
	}

	int GetHandle()
	{
		return handle;
	}

private:
	int handle;
};
