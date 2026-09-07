#pragma once

#include "IRenderer.h"
#include "IGPUConnecter.h"

class ServiceLocator
{
	friend class ResourceManager;
	friend class Renderer;
	friend class ApplicationManager;

private:
	// 取得用関数
	static IRenderer* GetRenderer() { return renderer; }
	static IGPUConnecter* GetGPUConnecter() { return gpuConnecter; }

	// セット用関数
	static void SetRenderer(IRenderer* _renderer) { renderer = _renderer; }
	static void SetGPUConnecter(IGPUConnecter* _gpuConnecter) { gpuConnecter = _gpuConnecter; }
private:
	inline static IRenderer* renderer{ nullptr };
	inline static IGPUConnecter* gpuConnecter{ nullptr };
};
