#pragma once

#include "ModelHandle.h"

struct FrameRendererComponent
{
public:
	FrameRendererComponent(ModelHandle _handle, std::uint32_t _frameIndex);
	FrameRendererComponent(const char* _modelPath,const char* _frameName);

	// コピーは禁止
	FrameRendererComponent(const FrameRendererComponent&) = delete;
	FrameRendererComponent& operator=(const FrameRendererComponent&) = delete;

	// ムーブで所有権を渡す
	FrameRendererComponent(FrameRendererComponent&& _other) noexcept;
	FrameRendererComponent& operator=(FrameRendererComponent&& _other) noexcept;

	ModelHandle GetHandle() const
	{
		return handle;
	}

	std::uint32_t GetFrameIndex() const
	{
		return frameIndex;
	}

	~FrameRendererComponent();
private:
	ModelHandle handle;
	std::uint32_t frameIndex;
};
