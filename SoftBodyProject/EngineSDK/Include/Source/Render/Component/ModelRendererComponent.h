#pragma once

#include "ModelHandle.h"

struct ModelRendererComponent
{
public:
	ModelRendererComponent(ModelHandle _handle);
	ModelRendererComponent(const char* _path);

	// コピーは禁止
	ModelRendererComponent(const ModelRendererComponent&) = delete;
	ModelRendererComponent& operator=(const ModelRendererComponent&) = delete;

	// ムーブで所有権を渡す
	ModelRendererComponent(ModelRendererComponent&& _other) noexcept;
	ModelRendererComponent& operator=(ModelRendererComponent&& _other) noexcept;

	ModelHandle GetHandle() const
	{
		return handle;
	}

	~ModelRendererComponent();
private:
	ModelHandle handle;
};
